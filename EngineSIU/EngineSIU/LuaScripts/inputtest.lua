turnSpeed = 80
MoveSpeed = 10
MaxMoveSpeed = 10

MouseHorizontalSensitive = 0.1
MouseVerticalSensitive = 0.001

---@type UPrimitiveComponent
local primitiveComp

---@type FBodyInstance
local bodyInstance

function BeginPlay()
    inputComp = obj:GetWorld():GetPlayerController():GetInputComponent()
    inputComp:BindTargetedKeyAction(FString("W"), obj, OnPressW)
    inputComp:BindTargetedKeyAction(FString("A"), obj, OnPressA)
    inputComp:BindTargetedKeyAction(FString("S"), obj, OnPressS)
    inputComp:BindTargetedKeyAction(FString("D"), obj, OnPressD)
    inputComp:BindTargetedKeyAction(FString("X"), obj, OnPressX)
    inputComp:BindTargetedMouseMoveAction(obj, OnMouseMove)
    inputComp:SetPossess()

    InitBodyInstance()
end

function InitBodyInstance()
    primitiveComp = obj:GetUPrimitiveComponent()
    bodyInstance = primitiveComp:GetBodyInstance()
    if (bodyInstance == nil) then return end
    bodyInstance.bLockXRotation = true
    bodyInstance.bLockYRotation = true
    bodyInstance.bLockZRotation = true
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
    if (bodyInstance:RayCast(FVector(0, 0, -1), 1 + KINDA_SMALL_NUMBER)) then
        bodyInstance:AddBodyVelocity(FVector(0, 0, 1))
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

    primitiveComp:AddRotation(FRotator(0, dx * MouseHorizontalSensitive, 0))

end

function Tick(dt)
    if (bodyInstance == nil) then InitBodyInstance() end
    ACamera = obj:GetWorld():GetPlayerController():GetCameraManager()
    ACamera:SetViewTarget(obj)
end

function BeginOverlap()
end

function EndOverlap()
end
