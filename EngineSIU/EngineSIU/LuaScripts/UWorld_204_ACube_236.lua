turnSpeed = 80
MoveSpeed = 10

function BeginPlay()
    LogDisplay("[BeginPlay]")
    inputComp = obj:GetUInputComponent()
    inputComp:BindTargetedAction(FString("W"), obj, OnPressW)
    inputComp:BindTargetedAction(FString("A"), obj, OnPressA)
    inputComp:BindTargetedAction(FString("S"), obj, OnPressS)
    inputComp:BindTargetedAction(FString("D"), obj, OnPressD)
    inputComp:SetPossess()
    SpawnActor("ACube")
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

function Tick(dt)
end

function BeginOverlap()
end

function EndOverlap()
end
