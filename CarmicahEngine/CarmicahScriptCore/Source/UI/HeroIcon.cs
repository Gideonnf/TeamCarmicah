using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class HeroIcon : Entity
    {
        public string heroPrefab = "Trap_1";
        public Entity heroEntity;

        void OnUpdate(float dt)
        {
            if (heroEntity == null) return;

            //CMConsole.Log("It shouldnt be here atm");

            //if (IsKeyHold(Keys.))
            Vector2 mousePos = Input.GetMousePos();
            heroEntity.Position = mousePos;



            if (Input.IsMouseReleased(MouseButtons.MOUSE_BUTTON_LEFT))
            {
                CMConsole.Log($"Destroying entity with {heroEntity.mID}");
                heroEntity.Destroy();
                heroEntity = null;
            }
        }

        void OnClick()
        {
            if (heroEntity != null) return;

            heroEntity = CreateGameObject(heroPrefab);
            CMConsole.Log($"Creating entity with {heroEntity.mID}");
        }

    }
}
