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
        

        void OnCreate()
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

        void OnUpdate(float dt)
        {
            if (trapEntity == null) return;

            //CMConsole.Log("It shouldnt be here atm");

            //if (IsKeyHold(Keys.))
            Vector2 mousePos = Input.GetMousePos();
            trapEntity.Position = mousePos;

            if (trapEntity.Position.x < 0.0f && flipped != true)
            {
                Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
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

            if(hovering)
            {

            }
        }

        void OnClick()
        {
            if (trapEntity != null) return;

            trapEntity = CreateGameObject(heroPrefab);
            //Not sure if this belongs here but shld work.
            heroBuildEntity.As<HeroBuild>().SetHeroType(type, heroPrefab, this);
            heroBuildEntity1.As<HeroBuild>().SetHeroType(type, heroPrefab, this);
            heroBuildEntity2.As<HeroBuild>().SetHeroType(type, heroPrefab, this);
            heroBuildEntity3.As<HeroBuild>().SetHeroType(type, heroPrefab, this);

        }

        void OnMouseHover()
        {
            if (!hovering)
            {
                Sound.PlaySFX("SFX__Magic");
                this.GetComponent<Renderer>().SetColour(1.5f, 1.5f, 1.5f);
                
                hovering = true;
            }
        }

        public void HeroBuilt()
        {
            Entity abilityBar = FindEntityWithName("UIBar");
            abilityBar.As<AbilityBar>().IconRemoved(this.As<BaseIcon>());
            Destroy();
        }

        void OnMouseExit()
        {
            hovering = false;
            this.GetComponent<Renderer>().SetColour(1.0f, 1.0f, 1.0f);
        }

    }
}
