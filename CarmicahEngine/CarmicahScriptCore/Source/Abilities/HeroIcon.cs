using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class HeroIcon : BaseIcon
    {
        public string heroPrefab = "Trap_1";
        //public Entity heroEntity;
        public string HeroBuild = "HeroBuild";
        public string HeroBuild1 = "HeroBuild_1";
        public string HeroBuild2 = "HeroBuild_2";
        public string HeroBuild3 = "HeroBuild_3";

        Entity heroBuildEntity;
        Entity heroBuildEntity1;
        Entity heroBuildEntity2;
        Entity heroBuildEntity3;

        // set these in script var
        public float xOffset = 0.0f;
        public float yOffset = 0.0f;

        public float mouseXOffset = 0.5f;

        public override void OnCreate()
        {
            heroBuildEntity = FindEntityWithName(HeroBuild);
            heroBuildEntity1 = FindEntityWithName(HeroBuild1);
            heroBuildEntity2 = FindEntityWithName(HeroBuild2);
            heroBuildEntity3 = FindEntityWithName(HeroBuild3);

            if(heroPrefab == "ShooterNPC")
            {
                type = AbilityType.SHOOTER;
                
            }
            else if(heroPrefab == "MageNPC")
            {
                type = AbilityType.MAGE;

                
            }
            else if(heroPrefab == "SpearNPC")
            {
                type = AbilityType.SPEAR;
            }
        }

        public override void OnUpdate(float dt)
        {
            if (trapEntity == null) return;

            //CMConsole.Log("It shouldnt be here atm");

            //if (IsKeyHold(Keys.))
            Vector2 mousePos = Input.GetMousePos();
            trapEntity.Position = mousePos + new Vector2(mouseXOffset, 0);

            if (trapEntity.Position.x < 0.0f && flipped != true)
            {
                Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                Vector2 Pos = trapEntity.GetComponent<Transform>().Position;
                trapEntity.GetComponent<Transform>().Position = new Vector2(Pos.x, Pos.y);
                flipped = true;
            }
            else if (trapEntity.Position.x > 0.0f && flipped == true)
            {
                Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                flipped = false;
            }

            if (Input.IsMouseReleased(MouseButtons.MOUSE_BUTTON_LEFT))
            {
                //CMConsole.Log($"Destroying entity with {heroEntity.mID}");
                trapEntity.Destroy();
                trapEntity = null;
                flipped = false;
            }
        }

        public override void OnClick()
        {
            if (trapEntity != null) return;

            trapEntity = CreateGameObject(heroPrefab);
            //if(type == AbilityType.SPEAR)
            //{
            //    Vector2 Pos = trapEntity.GetComponent<Transform>().Position;
            //    Pos.x -= 0.5f;
            //    trapEntity.GetComponent<Transform>().Position = Pos;
            //    CMConsole.Log("Shenanigans to the spear fella");

            //}

            //Not sure if this belongs here but shld work.
            heroBuildEntity.As<HeroBuild>().SetHeroType(type, heroPrefab, this);
            heroBuildEntity1.As<HeroBuild>().SetHeroType(type, heroPrefab, this);
            heroBuildEntity2.As<HeroBuild>().SetHeroType(type, heroPrefab, this);
            heroBuildEntity3.As<HeroBuild>().SetHeroType(type, heroPrefab, this);

        }

        public override void OnMouseEnter()
        {
            Sound.PlaySFX("Item_Hover", 0.4f);
            if (this.HasComponent<Renderer>())
                this.GetComponent<Renderer>().SetColour(1.5f, 1.5f, 1.5f);

        }

        public override void OnMouseHover()
        {
            if (mID == 0) return;

            //if (!hovering)
            //{
            //    
                
            //    hovering = true;
            //}
        }

        public void HeroBuilt()
        {
            Entity abilityBar = FindEntityWithName("UIBar");
            abilityBar.As<AbilityBar>().IconRemoved(this.As<BaseIcon>());
            CMConsole.Log("Deleting the icon from the bar!");
            Destroy();
        }

        public override void OnMouseExit()
        {
            //hovering = false;
            if(mID == 0) return;

            if(this.HasComponent<Renderer>())
            { this.GetComponent<Renderer>().SetColour(1.0f, 1.0f, 1.0f); }
        }

    }
}
