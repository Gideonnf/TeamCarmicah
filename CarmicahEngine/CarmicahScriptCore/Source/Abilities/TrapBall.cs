using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public enum IconType
    { 
        CANDY_ICON,
        SHOOTER_ICON,
        HONEY_ICON,
        MAGE_ICON,
        SPEAR_ICON
    }
    public class TrapBall : Entity
    {
        public int iconType;
        public Entity mainCamera;
        public float heightOffset = 12.0f;
        public float fallSpeed = 2.0f;

        bool touched = false;
        bool stop = false;
        public float timer = 0.0f;
        public float stopTime = 1.0f;
        float middleMin = 0.7f;
        float middleMax = 1.2f;
        float sideMin = 0.5f;
        float sideMax = 0.8f;
        public float lifeTime = 8.0f;
        // flash settings
        public float flashTime = 4.0f;
        public float flashInterval = 0.25f;
        public float flashTimer = 0.0f;
        bool invisible = false;
        Entity playerEntity;
        Entity powerControl;


        public override void OnCreate()
        {
           // CMConsole.Log("IOADNASNIODANSOID");
            mainCamera = FindEntityWithName("MainCamera");
            playerEntity = FindEntityWithName("mainCharacter");
            powerControl = FindEntityWithName("PowerUpControl");
            // Set position 
            // Position = new Vector2(0.0f, mainCamera.Position.y + heightOffset);
        }

        public override void OnUpdate(float dt)
        {
           // CMConsole.Log("does thsi function work");
            // fall down
            if (!stop)
            {
                Vector2 pos = Position;
                //if (pos.y <= 3.0f)
                //    CMConsole.Log($"{mID} is below 3.0");
                pos.y -= fallSpeed * dt;
                Position = pos;
            }
            else
            {
                timer += dt;
                float distance = playerEntity.Position.Distance(Position);
                //CMConsole.Log($"Distance {distance}");
                if (distance <= 1.0f)
                {
                    Entity abilityBar = FindEntityWithName("UIBar");

                    if (abilityBar.As<AbilityBar>().CreateIcon((IconType)iconType))
                    {
                        powerControl.As<PowerUpControl>().PowerUpDestroyed(this);
                        Destroy();
                        return;
                    }
                }

               // CMConsole.Log($"timer {timer}, vs {flashTime}");
                if (timer > flashTime)
                {
                    //CMConsole.Log("I AM IN HERE");
                    flashTimer += dt;
                    if (flashTimer > flashInterval)
                    {
                        flashTimer = 0.0f;
                        invisible = !invisible;
                        if (invisible)
                        {
                            GetComponent<Renderer>().SetAlpha(0.0f);
                        }
                        else
                        {
                            GetComponent<Renderer>().SetAlpha(1.0f);
                        }
                    }
                }

                if (timer > lifeTime)
                {
                    powerControl.As<PowerUpControl>().PowerUpDestroyed(this);
                    Destroy();
                    return;
                }
            }

            if (touched && !stop)
            {
                timer += dt;
                if (timer >= stopTime)
                {
                    //CMConsole.Log($"DASDASDASDA {stopTime} and {timer}");
                    timer = 0.0f;
                    stop = true;
                }
            }
        }

        public override void OnTriggerEnter(uint collidedEntity)
        {
            Entity entity = FindEntityWithID(collidedEntity);
           //CMConsole.Log($"Entity collided {collidedEntity}");
            // if colliding with player
            if (entity.GetTag() == "Player")
            {
                // add icon to the UI bar
                if (stop)
                {
                    Destroy();
                }
            }
            else if (entity.GetTag() == "TowerTopMiddle")
            {
                touched = true;
                stopTime = CMRand.Range(middleMin, middleMax);
                CMConsole.Log($"stop time {stopTime}");
            }
            else if (entity.GetTag() == "TowerTopSide")
            {
                touched = true;
                stopTime = CMRand.Range(sideMin, sideMax);
                CMConsole.Log($"stop time {stopTime}");
            }

        }

        // this doesnt work for player
        public override void OnTriggerStay(uint collidedEntity)
        {
            Entity entity = FindEntityWithID(collidedEntity);
            if (collidedEntity == 24)
            {
                CMConsole.Log("Colliding with player");
            }
            if (entity.GetTag() == "Player")
            {
                // add icon to the UI bar
                if (stop)
                {
                    Destroy();
                }
            }
        }
    }
}
