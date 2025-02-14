using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class TrapIcon : Entity
    {
        public string trapPrefab = "Trap_1";
        public Entity trapEntity;
        bool flipped = false;
        bool hovering = false;

        void OnUpdate(float dt)
        {
            if (trapEntity == null) return;

            //CMConsole.Log("It shouldnt be here atm");

            //if (IsKeyHold(Keys.))
            Vector2 mousePos = Input.GetMousePos();
            trapEntity.Position = mousePos;
            

            if(trapEntity.Position.x < 0.0f && flipped != true)
            {
                Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                flipped = true;
            }
            else if ( trapEntity.Position.x > 0.0f && flipped == true)
            {
                Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                flipped = false;
            }

            if (Input.IsMouseReleased(MouseButtons.MOUSE_BUTTON_LEFT))
            {
               // CMConsole.Log($"Destroying entity with {trapEntity.mID}");
                trapEntity.Destroy();
                trapEntity = null;
                flipped = false;
            }            
        }

        void OnClick()
        {
            if (trapEntity != null) return;

            trapEntity = CreateGameObject(trapPrefab);
            //CMConsole.Log($"Creating entity with {trapEntity.mID}");
        }

        void OnMouseHover()
        {
            if(!hovering)
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
