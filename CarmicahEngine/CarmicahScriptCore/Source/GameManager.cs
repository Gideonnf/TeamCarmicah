using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class GameManager : Entity
    {
        public float spawnTimer = 0.5f;
        public string MousePrefabName = "MouseGO";
        public float timer = 0.0f;
        public bool LeftOrRight = false;
        void OnCreate()
        {

        }

        void OnUpdate(float dt)
        {
            timer += dt;
            
            if (timer > spawnTimer)
            {
                timer = 0.0f;
                Entity mouseEntity = CreateGameObject(MousePrefabName);
                MouseAI mouseAI = mouseEntity.As<MouseAI>();
                mouseAI.isLeft = LeftOrRight;
                //Console.WriteLine($"Mouse is : {mouseAI.isLeft}");
                //Console.WriteLine($"LeftorRight is : {LeftOrRight}");
                LeftOrRight = !LeftOrRight;
                mouseAI.SetInitialPosition(); // Reset initial position

            }
        }

    }
}