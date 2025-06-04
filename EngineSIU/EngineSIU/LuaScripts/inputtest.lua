turnSpeed = 80
MoveSpeed = 300
MaxMoveSpeed = 300
JumpPower = 50

MouseHorizontalSensitive = 0.1
MouseVerticalSensitive = 0.001

local isAir = false

---@type table
local primitiveComps

---@type FBodyInstance
local bodyInstance


---@type UAnimSingleNodeInstance
local animInstance

local hasLanded = false

function BeginPlay()
    inputComp = obj:GetWorld():GetPlayerController():GetInputComponent()
    inputComp:BindTargetedKeyLuaAction(FString("W"), obj, OnPressW)
    inputComp:BindTargetedKeyLuaAction(FString("A"), obj, OnPressA)
    inputComp:BindTargetedKeyLuaAction(FString("S"), obj, OnPressS)
    inputComp:BindTargetedKeyLuaAction(FString("D"), obj, OnPressD)
    inputComp:BindTargetedKeyLuaAction(FString("X"), obj, OnPressX)
    inputComp:BindTargetedMouseMoveLuaAction(obj, OnMouseMove)
    inputComp:SetPossess()

    InitBodyInstance()

    skelComp = obj:GetUSkeletalMeshComponent()
    animInstance = skelComp:GetSingleNodeInstance()
    animInstance:SetAnimState(FString("Idle"))
end

function InitBodyInstance()
    PrintObj(obj:GetUPrimitiveComponents())
    primitiveComps = obj:GetUPrimitiveComponents():GetContainer()
    for k, v in pairs(primitiveComps) do
        if (v:GetBodyInstance() ~= nil) then
            bodyInstance = v:GetBodyInstance()
            break
        end
        LogDisplay(string.format("%s %s", ToString(k), ToString(v)))
    end
    if (bodyInstance == nil) then return end
    bodyInstance.bLockXRotation = true
    bodyInstance.bLockYRotation = true
    bodyInstance.bLockZRotation = true
    bodyInstance.InertiaTensorScale = FVector(10000, 10000, 10000)
    bodyInstance:UpdateBodyProperty()
end

function EndPlay()
end

function OnOverlap(OtherActor)
end

function OnPressW(dt)
    if (bodyInstance:GetBodyVelocity():Length() < MaxMoveSpeed) then
        bodyInstance:AddBodyVelocity(obj:GetActorForwardVector() * dt * MoveSpeed)
    end
end

function OnPressS(dt)
    if (bodyInstance:GetBodyVelocity():Length() < MaxMoveSpeed) then
        bodyInstance:AddBodyVelocity(obj:GetActorForwardVector() * -dt * MoveSpeed)
    end
end

function OnPressA(dt)
    if (bodyInstance:GetBodyVelocity():Length() < MaxMoveSpeed) then
        bodyInstance:AddBodyVelocity(obj:GetActorRightVector() * -dt * MoveSpeed)
    end
end

function OnPressD(dt)
    if (bodyInstance:GetBodyVelocity():Length() < MaxMoveSpeed) then
        bodyInstance:AddBodyVelocity(obj:GetActorRightVector() * dt * MoveSpeed)
    end
end

function OnPressX(dt)
    if (not isAir) then
        bodyInstance:AddBodyVelocity(FVector(0, 0, 1 * JumpPower))
        animInstance:SetAnimState(FString("Jump"))
        isAir = true
        hasLanded = false
    end
end

function OnMouseMove(dx, dy)
    -- local rot = FRotator(0, 0, dx * MouseHorizontalSensitive)
    -- bodyInstance:SetBodyAngularVelocity(rot)
    -- local rot = bodyInstance:GetBodyRotation()
    -- local prevrot = rot
    -- rot.Yaw = rot.Yaw + dx * MouseHorizontalSensitive
    -- bodyInstance:SetBodyRotation(rot)
    -- rot = bodyInstance:GetBodyRotation()
    -- -- PrintObj(string.format("%s", "asdf"))
    -- -- GetActorByName(ToString(rot))
    -- LogDisplay(string.format("%s %s", ToString(prevrot), ToString(rot)))

    obj:AddActorRotation(FRotator(0, dx * MouseHorizontalSensitive, 0))

end

function Tick(dt)
    if (bodyInstance == nil) then InitBodyInstance() end

    -- 바닥 감지
    local hit = bodyInstance:RayCast(FVector(0, 0, -1), 44)

    if (hit) then
        if (isAir and not hasLanded) then
            animInstance:SetAnimState(FString("Idle"))
            hasLanded = true
        end
        isAir = false
    else
        if (not isAir) then
            -- 공중으로 이륙할 때만 한 번 Jump 애니메이션 세팅
            animInstance:SetAnimState(FString("Jump"))
        end
        isAir = true
        hasLanded = false
    end

    -- 걸음 애니메이션 전환
    local velocity = bodyInstance:GetBodyVelocity()
    if (velocity:Length() > KINDA_SMALL_NUMBER and not isAir) then
        if (animInstance:GetCurrentState() == FString("Idle")) then
            animInstance:SetAnimState(FString("Walking"))
        end
    elseif (velocity:Length() < KINDA_SMALL_NUMBER and not isAir) then
        if (animInstance:GetCurrentState() == FString("Walking")) then
            animInstance:SetAnimState(FString("Idle"))
        end
    end
end


function BeginOverlap()
end

function EndOverlap()
end
