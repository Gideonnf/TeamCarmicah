using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class HeroIcon : Entity
    {
        public string heroPrefab = "Trap_1";
        public Entity heroEntity;
        bool flipped;
        bool hovering = false;
        

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
                CMConsole.Log($"Destroying entity with {heroEntity.mID}");
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
            CMConsole.Log($"Creating entity with {heroEntity.mID}");
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

        void OnMouseExit()
        {
            hovering = false;
            this.GetComponent<Renderer>().SetColour(1.0f, 1.0f, 1.0f);
        }

    }
}
