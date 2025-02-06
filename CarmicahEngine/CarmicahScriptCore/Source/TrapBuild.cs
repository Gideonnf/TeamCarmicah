using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
   public class TrapBuild : Entity
    {
        public string TrapPrefabName = "Trap_1";
        public string CandyAnim = "CandyCone";


        public string TrapTranslucentPrefab = "TrapTranslucent";
        public bool IsLeft = false;
        public float depthVal = 3.45f;
        // Radius to detect approaching mice
        public float DetectionRadius = 2.0f; 


        Entity translucentTrap;
        Entity trapEntity;
        Entity TrapIcon;
        bool hovering = false;
        bool built = false;
        bool isActive = false;
        GameManager gameManager;

        public void OnCreate()
        {
            
            TrapIcon = FindEntityWithName("TrapIcon");
            gameManager = FindEntityWithName("GameManager").As<GameManager>();



        }

        public void OnUpdate(float dt)
        {
            // Handle trap icon/translucent preview logic
            if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity != null)
            {
                if (translucentTrap == null && built == false)
                {
                    translucentTrap = CreateGameObject(TrapTranslucentPrefab);
                    translucentTrap.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                    translucentTrap.GetComponent<Transform>().Depth = depthVal;
                    if (IsLeft)
                    {
                        Vector2 scale = translucentTrap.GetComponent<Transform>().Scale;
                        translucentTrap.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                    }
                }
            }
            else if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity == null)
            {
                if (translucentTrap != null)
                {
                    translucentTrap.Destroy();
                    translucentTrap = null;

                    // Build trap if hovering over valid spot
                    if (hovering && trapEntity == null)
                    {
                        built = true;
                        trapEntity = CreateGameObject(TrapPrefabName);
                        trapEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                        trapEntity.GetComponent<Transform>().Depth = depthVal;
                        Sound.PlaySFX("trap_placement", 0.5f);

                        if (IsLeft)
                        {
                            Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                            trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                        }
                    }
                }
            }
            if (built && trapEntity != null)
            {
                CheckForMice();
            }
        }

        void CheckForMice()
        {
            if (gameManager == null) return;

            // Get the lists of mice based on which side the trap is on
            List<MouseAI> miceToCheck = IsLeft ? gameManager.mouseEntitiesLeft : gameManager.mouseEntitiesRight;

            bool mouseNearby = false;
            foreach (MouseAI mouse in miceToCheck)
            {
                if (mouse != null)
                {
                    // Calculate distance between mouse and trap
                    Vector2 mousePos = mouse.Position;
                    Vector2 trapPos = trapEntity.GetComponent<Transform>().Position;
                    float distance = (float)Math.Sqrt(
                        Math.Pow(mousePos.x - trapPos.x, 2) +
                        Math.Pow(mousePos.y - trapPos.y, 2)
                    );

                    // If mouse is within detection radius
                    if (distance <= DetectionRadius)
                    {
                        mouseNearby = true;
                        break;
                    }
                }
            }

            // Update trap animation state
            if (mouseNearby && !isActive)
            {
                isActive = true;
                if (trapEntity.HasComponent<Animation>())
                {
                    trapEntity.ChangeAnim(CandyAnim); 
                }
            }
            else if (!mouseNearby && isActive)
            {
                isActive = false;
            }
        }

        public void OnMouseEnter()
        {
            hovering = true;
        }

        public void OnMouseHover()
        {
            hovering = true;
        }

        public void OnMouseExit()
        {
            hovering = false;
        }
    }
}
