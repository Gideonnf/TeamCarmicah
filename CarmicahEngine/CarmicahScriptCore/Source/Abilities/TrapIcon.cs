using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class TrapIcon : BaseIcon
    {
        public float trapxOffset = 1.6f;
        public float trapyOffset = 0.0f;
        public bool cooldown = false;
        public float cooldownTime = 3.0f;
        //public string TrapBuild = "TrapBuild";
        //public string TrapBuild1 = "TrapBuild_1";
        //public string TrapBuild2 = "TrapBuild_2";
        //public string TrapBuild3 = "TrapBuild_3";
        public bool activeTrapIcon = false;

        float timer;
        //Entity trapBuildEntity;
        //Entity trapBuildEntity1;
        //Entity trapBuildEntity2;
        //Entity trapBuildEntity3;

        Vector2 minScale = new Vector2(0.05f, 0.05f);
        Vector2 maxScale;

        public override void OnCreate()
        {
            // use its current scale as the max
            maxScale = Scale;
            waveSystem = FindEntityWithName(WaveSystemObject);
            //trapBuildEntity = FindEntityWithName(TrapBuild);
            //trapBuildEntity1 = FindEntityWithName(TrapBuild1);
            //trapBuildEntity2 = FindEntityWithName(TrapBuild2);
            //trapBuildEntity3 = FindEntityWithName(TrapBuild3);

            if (actualTrapPrefab == "CandyConeTrap")
            {
                type = AbilityType.CANDY_CONE;
            }
            else if (actualTrapPrefab == "HoneyTrap")
            {
                type = AbilityType.HONEY;
            }
            else if (actualTrapPrefab == "JellybeanTrap")
            {
                type = AbilityType.JELLYBEAN;
            }
        }

        public override void OnUpdate(float dt)
        {
            if (cooldown)
            {
                //CMConsole.Log("On COoldown");
                timer += dt;
                float progress = 1 - (timer / cooldownTime);
                Scale = Vector2.lerp(maxScale, minScale, progress);

                if (timer > cooldownTime)
                {
                    timer = 0.0f;
                    cooldown = false;
                }
            }

            if (trapEntity == null) return;

            //CMConsole.Log("It shouldnt be here atm");

            //if (IsKeyHold(Keys.))
            Vector2 mousePos = Input.GetMousePos();
            if (!flipped)
                trapEntity.Position = new Vector2(mousePos.x + trapxOffset, mousePos.y);
            else
                trapEntity.Position = new Vector2(mousePos.x - trapxOffset, mousePos.y);


            if (trapEntity.Position.x < 0.0f && flipped != true)
            {
                Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                //trapEntity.Position = new Vector2(mousePos.x + trapOffset, mousePos.y);
                flipped = true;
            }
            else if ( trapEntity.Position.x > 0.0f && flipped == true)
            {
                Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
               // trapEntity.Position = new Vector2(mousePos.x - trapOffset, mousePos.y);
                flipped = false;
            }

            if (Input.IsMouseReleased(MouseButtons.MOUSE_BUTTON_LEFT))
            {
               // CMConsole.Log($"Destroying entity with {trapEntity.mID}");
                trapEntity.Destroy();
                trapEntity = null;
                flipped = false;
                activeTrapIcon = false;
            }            
        }

        public void ActivateCooldown()
        {
            if (waveSystem.As<WaveSystem>().waveStart)
            {
                cooldown = true;
            }

        }

        public override void OnClick()
        {
            if (trapEntity != null) return;


            if (cooldown == false)
            {
                trapEntity = CreateGameObject(fakeTrapPrefab);
                activeTrapIcon = true;
                //CMConsole.Log($"Actual trap prefab name {actualTrapPrefab}");

                Entity[] trapBuildSpots = FindEntitiesWithTag("TrapBuild");
                for(int i = 0; i < trapBuildSpots.Length; i++)
                {
                    trapBuildSpots[i].As<TrapBuild>().SetTrapType(type, actualTrapPrefab, fakeTrapPrefab, this);
                }
                //Sets the translucent for the trap type
                //trapBuildEntity.As<TrapBuild>().SetTrapType(type, actualTrapPrefabName, fakeTrapPrefab);
                //trapBuildEntity1.As<TrapBuild>().SetTrapType(type, actualTrapPrefabName, fakeTrapPrefab);
                //trapBuildEntity2.As<TrapBuild>().SetTrapType (type, actualTrapPrefabName, fakeTrapPrefab);
                //trapBuildEntity3.As<TrapBuild>().SetTrapType(type, actualTrapPrefabName, fakeTrapPrefab);
            }
            //CMConsole.Log($"Creating entity with {trapEntity.mID}");
        }

        public override void OnMouseEnter()
        {
            Sound.PlaySFX("Item_Hover", 0.4f);
            if (this.HasComponent<Renderer>())
                this.GetComponent<Renderer>().SetColour(1.5f, 1.5f, 1.5f);

        }


        public override void OnMouseHover()
        {

        }

        public override void OnMouseExit()
        {
            //hovering = false;
            if(this.HasComponent<Renderer>())
            {
                this.GetComponent<Renderer>().SetColour(1.0f, 1.0f, 1.0f);
            }
        }

        public void TrapBuilt()
        {
            Entity abilityBar = FindEntityWithName("UIBar");
            abilityBar.As<AbilityBar>().IconRemoved(this.As<BaseIcon>());
            Destroy();
        }
    }
}
