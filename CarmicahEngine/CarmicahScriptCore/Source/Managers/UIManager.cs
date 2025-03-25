using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    //public enum GameMenuStates
    //{

    //}
    public class UIState
    {
        public string menuName;
        public Entity UIEntity;
        public Vector2 hiddenPos;
        public Vector2 mainPos;

        public UIState(string name, Entity uiEntity)
        {
            menuName = name;
            UIEntity = uiEntity;
        }
    }
    public class UIManager : Entity
    {
        public List<UIState> states;
        public UIState currMenu;
        public UIState prevMenu;

        public override void OnCreate()
        {
            states = new List<UIState>();
        }

        public void CreateMenu(string prefabName)
        {
            Entity uiEntity = CreateGameObject(prefabName);
            // check if the menu already exist
            foreach(UIState state in states)
            {
                // if the menu has already been created before then return 
                if (state.menuName == prefabName)
                {
                    return;
                }
            }


            UIState newMenu = new UIState(prefabName, uiEntity);
            newMenu.hiddenPos = new Vector2(5000, 5000); // just throw it somewhere far as fk
            newMenu.mainPos = newMenu.UIEntity.Position;


            if (currMenu == null)
            {
                currMenu = newMenu;
            }
            else
            {
                prevMenu = currMenu;

                // move the prev menu to somewhere far 
                prevMenu.UIEntity.Position = prevMenu.hiddenPos;

                currMenu = newMenu;
                //prevMenu.UIEntity;
            }

            states.Add(newMenu);
        }

        public void CloseMenu(Entity menuEntity)
        {
            foreach (UIState state in states)
            {
                if (state.UIEntity == menuEntity)
                {
                    states.Remove(state);

                    // if the currMenu is the UI screen that is closing
                    if (currMenu == state && prevMenu != null)
                    {
                        currMenu = prevMenu;
                        // move the previous UI screen back to the main position
                        currMenu.UIEntity.Position = currMenu.mainPos;

                        // no more prev menu
                        prevMenu = null;
                    }

                    break;
                }
            }
        }
    }
}
