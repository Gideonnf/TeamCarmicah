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

        public string HealAnim;
        public string IdleAnim;
        public string WalkAnim;

        public bool isWalking = true;
        
        void OnCreate()
        {

        }

        void ToggleWalkAnim()
        {
            if (!isWalking)
            {
                isWalking = true;
                ChangeAnim(WalkAnim);
            }
        }

        void ToggleIdle()
        {
            if (isWalking)
            {
                isWalking = false;
                ChangeAnim(IdleAnim);
            }
        }

        void ToggleHeal()
        {
            isWalking = false;
            ChangeAnim(HealAnim);
        }

        void OnUpdate(float dt)
        {
            timer += dt;

            if (Input.IsKeyHold(Keys.KEY_W))
            {
                ToggleWalkAnim();

                PlaySoundEffect("walk");
                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, 1), 2.0f);
            }
            else if (Input.IsKeyHold(Keys.KEY_A))
            {
                ToggleWalkAnim();

                PlaySoundEffect("walk");

                Vector2 scale = Scale;
                if (scale.x < 0)
                {
                    scale.x *= -1;
                }

                Scale = scale;

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(-1, 0), 2.0f);
            }
            else if (Input.IsKeyHold(Keys.KEY_S))
            {
                ToggleWalkAnim();

                PlaySoundEffect("walk");

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, -1), 2.0f);
            }
            else if (Input.IsKeyHold(Keys.KEY_D))
            {
                ToggleWalkAnim();

                PlaySoundEffect("walk");

                Vector2 scale = Scale;
                if (scale.x > 0)
                {
                    scale.x *= -1;
                }

                Scale = scale;

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(1,0), 2.0f);
            }
            else
            {
                ToggleIdle();
            }
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
