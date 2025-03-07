using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class HeroIcon : Entity
    {
        public string heroPrefab = "Trap_1";
        public Entity heroEntity;
        public string HeroBuild = "HeroBuild";
        public string HeroBuild1 = "HeroBuild_1";
        public string HeroBuild2 = "HeroBuild_2";
        public string HeroBuild3 = "HeroBuild_3";

        Entity heroBuildEntity;
        Entity heroBuildEntity1;
        Entity heroBuildEntity2;
        Entity heroBuildEntity3;
        bool flipped;
        HeroType type;
        bool hovering = false;
        

        void OnCreate()
        {
            heroBuildEntity = FindEntityWithName(HeroBuild);
            heroBuildEntity1 = FindEntityWithName(HeroBuild1);
            heroBuildEntity2 = FindEntityWithName(HeroBuild2);
            heroBuildEntity3 = FindEntityWithName(HeroBuild3);

            if(heroPrefab == "ShooterNPC")
            {
                type = HeroType.SHOOTER;
            }
            else if(heroPrefab == "MageNPC")
            {
                type = HeroType.MAGE;
            }
            else if(heroPrefab == "SpearNPC")
            {
                type = HeroType.SPEAR;
            }
        }

        void OnUpdate(float dt)
        {
            if (heroEntity == null) return;

            //CMConsole.Log("It shouldnt be here atm");

            //if (IsKeyHold(Keys.))
            Vector2 mousePos = Input.GetMousePos();
            heroEntity.Position = mousePos;

            if (heroEntity.Position.x < 0.0f && flipped != true)
            {
                Vector2 scale = heroEntity.GetComponent<Transform>().Scale;
                heroEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                flipped = true;
            }
            else if (heroEntity.Position.x > 0.0f && flipped == true)
            {
                Vector2 scale = heroEntity.GetComponent<Transform>().Scale;
                heroEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                flipped = false;
            }

            if (Input.IsMouseReleased(MouseButtons.MOUSE_BUTTON_LEFT))
            {
                //CMConsole.Log($"Destroying entity with {heroEntity.mID}");
                heroEntity.Destroy();
                heroEntity = null;
                flipped = false;
            }

            if(hovering)
            {

            }
        }

        void OnClick()
        {
            if (heroEntity != null) return;

            heroEntity = CreateGameObject(heroPrefab);
            //Not sure if this belongs here but shld work.
            heroBuildEntity.As<HeroBuild>().SetHeroType(type, heroPrefab);
            heroBuildEntity1.As<HeroBuild>().SetHeroType(type, heroPrefab);
            heroBuildEntity2.As<HeroBuild>().SetHeroType(type, heroPrefab);
            heroBuildEntity3.As<HeroBuild>().SetHeroType(type, heroPrefab);

        }

        void OnMouseHover()
        {
            if (!hovering)
            {
                Sound.PlaySFX(this.mID, "SFX__Magic");
                this.GetComponent<Renderer>().SetColour(1.5f, 1.5f, 1.5f);
                
                hovering = true;
            }
        }

        void OnMouseExit()
        {
            hovering = false;
            this.GetComponent<Renderer>().SetColour(1.0f, 1.0f, 1.0f);
        }

    }
}
