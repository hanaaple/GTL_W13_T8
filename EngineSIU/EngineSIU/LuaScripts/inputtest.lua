turnSpeed = 80
MoveSpeed = 10
MaxMoveSpeed = 10

MouseHorizontalSensitive = 0.1
MouseVerticalSensitive = 0.001

local isAir = false

---@type table
local primitiveComps

---@type FBodyInstance
local bodyInstance


---@type UAnimSingleNodeInstance
local animInstance

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
    bodyInstance.InertiaTensorScale = FVector(5000, 5000, 5000)
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
        bodyInstance:AddBodyVelocity(FVector(0, 0, 4.5))
        animInstance:SetAnimState(FString("Jump"))
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

    if (math.abs(bodyInstance:GetBodyVelocity().z) < KINDA_SMALL_NUMBER and bodyInstance:RayCast(FVector(0, 0, -1), 5 + KINDA_SMALL_NUMBER)) then
        isAir = false
        if (animInstance:GetCurrentState() == FString("Jump")) then
            animInstance:SetAnimState(FString("Idle"))
        end
    else
        isAir = true
    end

    if (bodyInstance:GetBodyVelocity():Length() > KINDA_SMALL_NUMBER and not isAir) then
        if (animInstance:GetCurrentState() == FString("Idle")) then
            animInstance:SetAnimState(FString("Walking"))
        end
    elseif (bodyInstance:GetBodyVelocity():Length() < KINDA_SMALL_NUMBER and not isAir) then
        if (animInstance:GetCurrentState() == FString("Walking")) then
            animInstance:SetAnimState(FString("Idle"))
        end
    end
    ACamera = obj:GetWorld():GetPlayerController():GetCameraManager()
    ACamera:SetViewTarget(obj)
    -- LogDisplay(animInstance:GetCurrentState():ToString())
end

function BeginOverlap()
end

function EndOverlap()
end
