using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class HeroBuild : Entity
    {
        public static string HeroPrefab = "";
        public float lane; // float cause i dont rmb if imgui can show script int vars yet and i lazy check now
       // public string TrapTranslucentPrefab = "TrapTranslucent";
        public bool IsLeft = false;
        public float depthVal;
        Entity translucentHero;
        Entity heroEntity;
        Entity shooterIcon;
        Entity mageIcon;
        HeroType type;
        bool hovering = false;
        bool built = false;

        public void OnCreate()
        {
            shooterIcon = FindEntityWithName("ShooterIcon");
            mageIcon = FindEntityWithName("MageIcon");
        }

        public void OnUpdate(float dt)
        {
            // incase hero entity gets destroyed, it needs to update to null here
            if (heroEntity != null && heroEntity.mID == 0)
            {
                heroEntity = null;
            }
            switch (type)
            {
                case HeroType.SHOOTER:
                    {
                        //CMConsole.Log("Carrying out Shooter Build");
                        if (shooterIcon != null && shooterIcon.As<HeroIcon>().heroEntity != null)
                        {

                            if (translucentHero == null && built == false)
                            {
                                //CMConsole.Log("It shouldnt be here atm");
                                translucentHero = CreateGameObject(HeroPrefab);

                                // change the opacity here
                                translucentHero.GetComponent<Renderer>().SetAlpha(0.3f);
                                translucentHero.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                                // translucentHero.GetComponent<Transform>().Depth = depthVal;
                                if (IsLeft)
                                {
                                    Vector2 scale = translucentHero.GetComponent<Transform>().Scale;
                                    translucentHero.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                                }
                            }
                        }

                        else if (shooterIcon != null && shooterIcon.As<HeroIcon>().heroEntity == null)
                        {
                            if (translucentHero != null)
                            {
                                translucentHero.Destroy();
                                translucentHero = null;

                                // if the player let go when its hovering a build spot
                                if (hovering && heroEntity == null)
                                {
                                    // build a trap
                                    built = true;
                                    heroEntity = CreateGameObject(HeroPrefab);
                                    heroEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                                    heroEntity.As<HeroAI>().lane = (int)lane;
                                    heroEntity.As<HeroAI>().active = true;
                                    heroEntity.As<HeroAI>().type = HeroType.SHOOTER;
                                    if (IsLeft)
                                    {
                                        heroEntity.As<HeroAI>().IsLeft = true;
                                    }
                                    else
                                    {
                                        heroEntity.As<HeroAI>().IsLeft = false;
                                    }
                                    Sound.PlaySFX("trap_placement", 0.5f);
                                    // heroEntity.GetComponent<Transform>().Depth = depthVal;

                                    if (IsLeft)
                                    {
                                        Vector2 scale = heroEntity.GetComponent<Transform>().Scale;
                                        heroEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                                    }

                                    GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                                    gm.NewNPC(heroEntity);

                                }
                            }
                        }
                        break;
                    }
                case HeroType.MAGE:
                    {
                        //CMConsole.Log("MageNPC Build");
                        if (mageIcon != null && mageIcon.As<HeroIcon>().heroEntity != null)
                        {
                            CMConsole.Log("Condition 1 fufilled");
                            if (translucentHero == null && built == false)
                            {
                                CMConsole.Log("Should be making the transparent fella here");
                                //CMConsole.Log("It shouldnt be here atm");
                                translucentHero = CreateGameObject(HeroPrefab);

                                // change the opacity here
                                translucentHero.GetComponent<Renderer>().SetAlpha(0.3f);
                                translucentHero.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                                // translucentHero.GetComponent<Transform>().Depth = depthVal;
                                if (IsLeft)
                                {
                                    Vector2 scale = translucentHero.GetComponent<Transform>().Scale;
                                    translucentHero.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                                }
                            }
                        }

                        else if (mageIcon != null && mageIcon.As<HeroIcon>().heroEntity == null)
                        {
                            if (translucentHero != null)
                            {
                                translucentHero.Destroy();
                                translucentHero = null;

                                // if the player let go when its hovering a build spot
                                if (hovering && heroEntity == null)
                                {
                                    // build a trap
                                    built = true;
                                    heroEntity = CreateGameObject(HeroPrefab);
                                    heroEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                                    heroEntity.As<HeroAI>().lane = (int)lane;
                                    heroEntity.As<HeroAI>().active = true;
                                    heroEntity.As<HeroAI>().type = HeroType.MAGE;
                                    if (IsLeft)
                                    {
                                        heroEntity.As<HeroAI>().IsLeft = true;
                                    }
                                    else
                                    {
                                        heroEntity.As<HeroAI>().IsLeft = false;
                                    }
                                    Sound.PlaySFX("trap_placement", 0.5f);
                                    // heroEntity.GetComponent<Transform>().Depth = depthVal;

                                    if (IsLeft)
                                    {
                                        Vector2 scale = heroEntity.GetComponent<Transform>().Scale;
                                        heroEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                                    }

                                    GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                                    gm.NewNPC(heroEntity);

                                }
                            }
                        }
                        break;
                    }
            }
        }


        public void SetHeroType(HeroType heroType, string heroPrefabName)
        {
            CMConsole.Log("Setting heroType and prefab to" + heroPrefabName);
            type = heroType;
            HeroPrefab = heroPrefabName;
        }

        public void OnMouseEnter()
        {
            hovering = true;
        }

        public void OnMouseHover()
        {
            hovering = true;
        }

        public void OnMouseExit()
        {
            hovering = false;
        }
    }
}
