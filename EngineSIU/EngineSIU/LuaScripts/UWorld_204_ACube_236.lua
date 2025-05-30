turnSpeed = 80
MoveSpeed = 10

function BeginPlay()
    print("Begin")
end

function EndPlay()
    print("[EndPlay]")
    unBindController("W", HandleW)
    unBindController("A", HandleA)
    unBindController("S", HandleS)
    unBindController("D", HandleD)
end

function OnOverlap(OtherActor)
end

function InitializeLua()
    controller("W", OnPressW)    
    controller("S", OnPressS)
    controller("A", OnPressA)
    controller("D", OnPressD)
end

function OnPressW(dt)
    local currentPos = actor.Location
    currentPos = currentPos + actor:Forward() * dt * MoveSpeed
    actor.Location = currentPos
end

function OnPressS(dt)
    local currentPos = actor.Location
    currentPos = currentPos - actor:Forward() * dt * MoveSpeed
    actor.Location = currentPos    
end

function OnPressA(dt)
    local rot = actor.Rotator
    rot.Yaw = rot.Yaw - turnSpeed * dt
    actor.Rotator = rot
end

function OnPressD(dt)
    local rot = actor.Rotator
    rot.Yaw = rot.Yaw + turnSpeed * dt
    actor.Rotator = rot
end

function Tick(dt)
    -- actor:SetLocation(newPos)
    -- actor:SetLocation(FVector(200, 0, 0))
    -- UE_LOG("Error", "actor:SetLocation")

    -- actor.Location = FVector(0, 0, 0)
    -- actor.Location.X = actor.Location.X + dt
end

function BeginOverlap()
    print("BeginOverlap")
end

function EndOverlap()
    print("EndOverlap")
end
