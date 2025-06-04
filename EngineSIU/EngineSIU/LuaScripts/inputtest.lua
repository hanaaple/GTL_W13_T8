turnSpeed = 80
MoveSpeed = 300
MaxMoveSpeed = 300
JumpPower = 50000

MouseHorizontalSensitive = 0.1
MouseVerticalSensitive = 0.001

-- jumpState: "Idle", "JumpUp", "Falling", "JumpDown", "Walking" 중 하나
local jumpState = "Idle"

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
    if jumpState == "Idle" or jumpState == "Walking" then
        local velLen = bodyInstance:GetBodyVelocity():Length()
        if (velLen < MaxMoveSpeed) then
            bodyInstance:AddBodyVelocity(obj:GetActorForwardVector() * dt * MoveSpeed)
        end
    end 
end

function OnPressS(dt)
    if jumpState == "Idle" or jumpState == "Walking" then
        local velLen = bodyInstance:GetBodyVelocity():Length()
        if (velLen < MaxMoveSpeed) then
            bodyInstance:AddBodyVelocity(obj:GetActorForwardVector() * -dt * MoveSpeed)
        end
    end
end

function OnPressA(dt)
    if jumpState == "Idle" or jumpState == "Walking" then
        local velLen = bodyInstance:GetBodyVelocity():Length()
        if (velLen < MaxMoveSpeed) then
            bodyInstance:AddBodyVelocity(obj:GetActorRightVector() * -dt * MoveSpeed)
        end
    end
end

function OnPressD(dt)
    if jumpState == "Idle" or jumpState == "Walking" then
        local velLen = bodyInstance:GetBodyVelocity():Length()
        if (velLen < MaxMoveSpeed) then
            bodyInstance:AddBodyVelocity(obj:GetActorRightVector() * dt * MoveSpeed)
        end
    end
end

function OnPressX(dt)
    -- jumpState가 "Idle"(혹은 "Walking")일 때만 점프 허용
    
    if (jumpState == "Idle" or jumpState == "Walking") then
        -- 위쪽으로 속도 변화 주기
        LogWarning(string.format("OnJump"))
        bodyInstance:AddBodyImpulse(FVector(0, 0, 1 * JumpPower))
        -- 점프 애니메이션 시작: JumpUp
        animInstance:SetAnimState(FString("JumpUp"))
        animInstance:SetLooping(false)
        animInstance:SetPlaying(true)
        jumpState = "JumpUp"
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

    -- 1) 먼저 'JumpUp' 상태인지 확인
    if jumpState == "JumpUp" then
        -- 애니메이션 재생 시간과 전체 길이를 가져와서 비교
        local currentTime = animInstance:GetAnimCurrentTime()
        local totalTime   = animInstance:GetAnimLength()
        -- 아직 재생 중이면 아무 것도 하지 않고 종료
        if (currentTime < totalTime) then
            if(currentTime == 0) then
                -- 애니메이션이 완전히 끝났으면 'Falling'으로 전환
                animInstance:SetAnimState(FString("Falling"))
                animInstance:SetLooping(true)
                animInstance:SetPlaying(true)
                jumpState = "Falling"
            end
            return
        end

        -- 애니메이션이 완전히 끝났으면 'Falling'으로 전환
        animInstance:SetAnimState(FString("Falling"))
        animInstance:SetLooping(true)
        animInstance:SetPlaying(true)
        jumpState = "Falling"
        return
    end

    -- 1) 바닥 감지 (짧은 RayCast)
    local halfHeight = 0      -- (예시: 캡슐 콜라이더 half-height)
    local epsilon    = 2       -- (예시: 2cm 여유)
    local rayLength  = halfHeight + epsilon

    local hit = bodyInstance:RayCast(obj:GetActorLocation(), FVector(0, 0, -1), rayLength)

    if (hit) then
        -- 지면에 닿아 있는 경우: Falling 상태일 때만 Idle로 전환
        if (jumpState == "Falling") then
            animInstance:SetAnimState(FString("Idle"))
            animInstance:SetLooping(true)
            animInstance:SetPlaying(true)
            jumpState = "Idle"
        end
    else
        if (jumpState == "Idle" or jumpState == "Walking") then
            -- 지면에서 벗어나면 바로 Falling으로 전환
            animInstance:SetAnimState(FString("Falling"))
            animInstance:SetLooping(true)
            animInstance:SetPlaying(true)
            jumpState = "Falling"
            return
        end
        -- jumpState == "Falling"인 경우 그대로 유지
    end

    -- 3) 지면 위/Idle 시에만 이동 애니메이션 처리
    if (jumpState == "Idle") then
        local velocity = bodyInstance:GetBodyVelocity()
        local speed = velocity:Length()
        if (speed > KINDA_SMALL_NUMBER) then
            -- 움직이고 있으면 Walking
            if (animInstance:GetCurrentState() ~= FString("Walking")) then
                animInstance:SetAnimState(FString("Walking"))
                animInstance:SetLooping(true)
                animInstance:SetPlaying(true)
                jumpState = "Walking"
            end
        else
            -- 멈춰 있으면 Idle
            if (animInstance:GetCurrentState() ~= FString("Idle")) then
                animInstance:SetAnimState(FString("Idle"))
                animInstance:SetLooping(true)
                animInstance:SetPlaying(true)
                jumpState = "Idle"
            end
        end

    elseif (jumpState == "Walking") then
        -- 걷고 있는 상태에서 완전히 멈추면 Idle로 전환
        local velocity = bodyInstance:GetBodyVelocity()
        if (velocity:Length() < KINDA_SMALL_NUMBER) then
            animInstance:SetAnimState(FString("Idle"))
            animInstance:SetLooping(true)
            animInstance:SetPlaying(true)
            jumpState = "Idle"
        end
    end
end



function BeginOverlap()
end

function EndOverlap()
end
