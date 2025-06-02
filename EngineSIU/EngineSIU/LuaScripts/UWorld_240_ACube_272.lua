turnSpeed = 80
MoveSpeed = 50
AirMoveSpeed = 20
MaxMoveSpeed = 10

---@type boolean
local isGround

---@type UPrimitiveComponent
local primitiveComp

---@type FBodyInstance
local bodyInstance

---@type FVector
local MoveDirection = FVector(0, 0, 0)

function BeginPlay()
    inputComp = obj:GetWorld():GetPlayerController():GetInputComponent()
    inputComp:BindTargetedAction(FString("W"), obj, OnPressW)
    inputComp:BindTargetedAction(FString("A"), obj, OnPressA)
    inputComp:BindTargetedAction(FString("S"), obj, OnPressS)
    inputComp:BindTargetedAction(FString("D"), obj, OnPressD)
    inputComp:BindTargetedAction(FString("X"), obj, OnPressX)
    inputComp:SetPossess()

    primitiveComp = obj:GetUPrimitiveComponent()
    PrintObj(bodyInstance)
end

function EndPlay()
end

function OnOverlap(OtherActor)
end

function OnPressW(dt)
    MoveDirection.x = 1
end

function OnPressS(dt)
    MoveDirection.x = -1
end

function OnPressA(dt)    
    MoveDirection.y = -1
end

function OnPressD(dt)    
    MoveDirection.y = 1
end

function OnPressX(dt)
    if (isGround) then
        bodyInstance:AddBodyAccelation(FVector(0, 0, 350))
    end
end

function Tick(dt)
    if (bodyInstance == nil) then
        InitBodyInstance()
    end

    isGround = bodyInstance:RayCast(FVector(0, 0, -1), 1 + KINDA_SMALL_NUMBER)

    ---@type number
    local movespeed

    if (isGround) then
        movespeed = MoveSpeed
    else
        movespeed = AirMoveSpeed
    end

    if (bodyInstance:GetBodyVelocity():Length() < MaxMoveSpeed) then
        bodyInstance:AddBodyVelocity(MoveDirection:Normal(KINDA_SMALL_NUMBER) * movespeed * dt)
    end

    MoveDirection = FVector(0, 0, 0)
end

function BeginOverlap(OtherActor)
end

function EndOverlap(OtherActor)
end

function InitBodyInstance()
    bodyInstance = primitiveComp:GetBodyInstance()
    bodyInstance.bLockXRotation = true
    bodyInstance.bLockYRotation = true
    bodyInstance:UpdateBodyProperty()
end