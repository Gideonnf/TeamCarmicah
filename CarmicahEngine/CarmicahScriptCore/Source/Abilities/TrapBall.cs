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
        MAGE_ICON
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
        public float middleMin = 0.4f;
        public float middleMax = 1.0f;
        public float sideMin = 0.2f;
        public float sideMax = 0.5f;
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
                pos.y -= fallSpeed * dt;
                Position = pos;
            }
            else
            {
                float distance = playerEntity.Position.Distance(Position);
                //CMConsole.Log($"Distance {distance}");
                if (distance <= 1.0f)
                {
                    powerControl.As<PowerUpControl>().PowerUpDestroyed(this);
                    Entity abilityBar = FindEntityWithName("UIBar");

                    abilityBar.As<AbilityBar>().CreateIcon((IconType)iconType);

                    Destroy();
                }
            }

            if (touched)
            {
                timer += dt;
                if (timer >= stopTime)
                {
                   //CMConsole.Log($"DASDASDASDA {stopTime} and {timer}");
                    stop = true;
                }
            }
        }

        public override void OnTriggerEnter(uint collidedEntity)
        {
            Entity entity = FindEntityWithID(collidedEntity);
           CMConsole.Log($"Entity collided {collidedEntity}");
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
            }
            else if (entity.GetTag() == "TowerTopSide")
            {
                touched = true;
                stopTime = CMRand.Range(sideMin, sideMin);
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
