using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class SceneChanger : Entity
    {
        
        float sceneChangeTimer = -1f;
        Vector2 max;
        Vector2 min;
        Entity button;
        public string buttonType;
        string Animation0;
        public string Animation1;
        string nextScene;

        void OnCreate()
        {
            if (buttonType.ToLower() == "play")
            {
                button = FindEntityWithName("Play Button");

            }
            else if (buttonType.ToLower() == "quit")
            {
                button = FindEntityWithName("Quit Button");
            }



            //max.x = button.Position.x + 10.0f;
            //max.y = button.Position.y + 10.0f;
            //min.x = button.Position.x - 10.0f;
            //min.y = button.Position.y - 10.0f;
        }

        public void SetScene(string sceneName, float delay = 1.0f)
        {
            sceneChangeTimer = delay;
            nextScene = sceneName;
        }

        void OnUpdate(float dt)
        {

            //CheckHover();

            if (sceneChangeTimer > 0)
            {
                sceneChangeTimer -= dt;
                if (sceneChangeTimer <= 0)
                {
                    sceneChangeTimer = -1f;
                    if(nextScene == "quit")
                    {
                        Scene.CloseGame();
                    }
                    else
                    {

                        Scene.ChangeScene(nextScene);

                    }
                }
            }
        }

        //void CheckHover()
        //{
        //    Vector2 mousePos = Input.GetMousePos();

        //    if (mousePos.x < max.x && mousePos.y < max.y && mousePos.x > min.x && mousePos.y > min.y)
        //    {
        //        Console.WriteLine("Inside");
        //        ChangeAnim(Animation0);
        //    }
        //}

        public void OnMouseEnter()
        {
            if (buttonType.ToLower() == "play")
            {
                Animation0 = "Button_Play";

            }
            else if (buttonType.ToLower() == "quit")
            {
                Animation0 = "Button_Quit";
            }
            ChangeAnim(Animation0);
        }

        void OnClick()
        {
            Console.WriteLine($"Testing Play Button {mID}");
            Sound.PlaySFX("SFX_Button",0.5f);
            if(buttonType.ToLower() == "play")
            {
                nextScene = "Scene1";

            }else if(buttonType.ToLower() == "quit")
            {
                nextScene = "quit";
            }
                SetScene(nextScene);
        }


    }
}
