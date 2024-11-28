using CarmicahScriptCore.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Player : Entity
    {
        float timer = 0.5f;
        void OnCreate()
        {



            //if (isLeft)
            //{
            //    Position = waypointsLeft[currPoint];
            //}
            //else
            //{
            //    Position = waypointsRight[currPoint];
            //}

            //currPoint++;

            //// store original position
            //originalPos = Position;
        }

        void OnUpdate(float dt)
        {
            timer += dt;

            if (Input.IsKeyHold(Keys.KEY_W))
            {
                PlaySoundEffect("walk");
                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, 1), 2.0f);
            }
            if (Input.IsKeyHold(Keys.KEY_A))
            {
                PlaySoundEffect("walk");

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(-1, 0), 2.0f);
            }
            if (Input.IsKeyHold(Keys.KEY_S))
            {
                PlaySoundEffect("walk");

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, -1), 2.0f);
            }
            if (Input.IsKeyHold(Keys.KEY_D))
            {
                PlaySoundEffect("walk");

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(1,0), 2.0f);
            }
        }

        void OnCollide()
        {

        }

        void PlaySoundEffect(string name)
        {
            if (timer >= 0.5f)
            {
                Sound.PlaySFX(name);
                timer = 0.0f;
            }
        }
    }
}
