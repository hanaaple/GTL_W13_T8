turnSpeed       = 80
MoveSpeed       = 10
rotTime         = 0.5
bIsOverlapping  = false
bIsOverlapped = false
overlapElapsed  = 0
initialRoll      = 0

function BeginPlay()
    print("Begin")
end

function EndPlay()
    print("[EndPlay]")
end

function OnOverlap(OtherActor)
end

function InitializeLua()
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
    if bIsOverlapped then
        overlapElapsed = overlapElapsed + dt

        -- 경과 시간에 따른 회전 비율 (0~1)
        local t     = overlapElapsed / rotTime
        if t > 1 then t = 1 end

-- 초기값 + 최대 90도까지 보간
        local targetRoll = initialRoll + 90 * t
        local rot        = actor.Rotator
        rot.Roll          = targetRoll
        actor.Rotator     = rot

        -- 회전 완료 시 상태 초기화
        if overlapElapsed >= rotTime then
            bIsOverlapped = false
            bIsOverlapping = false
            overlapElapsed = 0
        end
    end
end

function BeginOverlap(OtherActor)
    print("BeginOverlap")
    bIsOverlapped = true
    bIsOverlapping = true
    overlapElapsed = 0
    -- 시작할 때의 Roll 값 저장
    initialRoll    = actor.Rotator.Roll
end

function EndOverlap(OtherActor)
    print("EndOverlap")
    bIsOverlapping = false
end
