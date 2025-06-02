turnSpeed = 80
MoveSpeed = 10
local VerticalSpeed = 0
local Gravity = 45

function BeginPlay()
    LogDisplay("[BeginPlay]")
    PrintObj(obj:GetWorld():GetPlayerController())
    inputComp = obj:GetWorld():GetPlayerController():GetInputComponent()
    inputComp:BindTargetedAction(FString("W"), obj, OnPressW)
    inputComp:BindTargetedAction(FString("A"), obj, OnPressA)
    inputComp:BindTargetedAction(FString("S"), obj, OnPressS)
    inputComp:BindTargetedAction(FString("D"), obj, OnPressD)
    inputComp:BindTargetedAction(FString("X"), obj, OnPressX)
    inputComp:SetPossess()



    -- SpawnActor("ACube")
end

function EndPlay()
    LogDisplay("[EndPlay]")
end

function OnOverlap(OtherActor)
end

function OnPressW(dt)
    local currentPos = obj:GetActorLocation()
    currentPos = currentPos + obj:GetActorForwardVector() * dt * MoveSpeed
    obj:SetActorLocation(currentPos)
end

function OnPressS(dt)
    local currentPos = obj:GetActorLocation()
    currentPos = currentPos - obj:GetActorForwardVector() * dt * MoveSpeed
    obj:SetActorLocation(currentPos)
end

function OnPressA(dt)
    local rot = obj:GetActorRotation()
    rot.Yaw = rot.Yaw - turnSpeed * dt
    obj:SetActorRotation(rot)
end

function OnPressD(dt)
    local rot = obj:GetActorRotation()
    rot.Yaw = rot.Yaw + turnSpeed * dt
    obj:SetActorRotation(rot)
end

function OnPressX(dt)
    local pos = obj:GetActorLocation()
    if (pos.z < KINDA_SMALL_NUMBER) then
        VerticalSpeed = 10
    end
end

function Tick(dt)
    local speed = FVector(0, 0, VerticalSpeed)
    VerticalSpeed = VerticalSpeed - Gravity * dt
    obj:AddActorLocation(speed * dt)

    local pos = obj:GetActorLocation()
    if (pos.z < 0) then
        pos.z = 0
        VerticalSpeed = 0
    end
    obj:SetActorLocation(pos)

    ACamera = obj:GetWorld():GetPlayerController():GetCameraManager()
    ACamera:SetViewTarget(obj)
end

function BeginOverlap()
end

function EndOverlap()
end
