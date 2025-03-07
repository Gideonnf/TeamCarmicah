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
        public Entity heroEntity;
        //Entity shooterIcon;
        //Entity mageIcon;
        AbilityType type;
        bool hovering = false;
        bool built = false;
        Entity heroIcon;

        public void OnCreate()
        {
            //shooterIcon = FindEntityWithName("ShooterIcon");
            //mageIcon = FindEntityWithName("MageIcon");
        }

        public void OnUpdate(float dt)
        {
            // incase hero entity gets destroyed, it needs to update to null here
            if (heroEntity != null && heroEntity.mID == 0)
            {
                heroEntity = null;
            }

            if (heroIcon != null && heroIcon.mID == 0)
            {
                if (translucentHero != null)
                {
                    //CMConsole.Log("This is sus");
                    translucentHero.Destroy();
                    translucentHero = null;
                }
                heroIcon = null;
            }

            //CMConsole.Log("Carrying out Shooter Build");
            if (heroIcon != null && heroIcon.As<BaseIcon>().trapEntity != null)
            {

                if (translucentHero == null && built == false)
                {
                    //CMConsole.Log("It shouldnt be here atm");
                    CMConsole.Log("Creating the translucent hero");
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

            else if (heroIcon != null && heroIcon.As<HeroIcon>().trapEntity == null)
            {
                if (translucentHero != null)
                {
                    CMConsole.Log("Deleting the translucent hero");
                    translucentHero.Destroy();
                    translucentHero = null;

                    // if the player let go when its hovering a build spot
                    if (hovering && heroEntity == null)
                    {
                        // build a trap
                        CMConsole.Log("Setting Built to true");
                        built = true;
                        heroEntity = CreateGameObject(HeroPrefab);
                        heroEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                        heroEntity.As<BaseNPC>().lane = (int)lane;
                        heroEntity.As<BaseNPC>().active = true;
                        heroEntity.As<BaseNPC>().npcType = heroIcon.As<HeroIcon>().type;
                        if (IsLeft)
                        {
                            heroEntity.As<BaseNPC>().IsLeft = true;
                        }
                        else
                        {
                            heroEntity.As<BaseNPC>().IsLeft = false;
                        }
                        Sound.PlaySFX(this.mID, "Trap_Placement_v2", 0.5f);
                        // heroEntity.GetComponent<Transform>().Depth = depthVal;

                        if (IsLeft)
                        {
                            Vector2 scale = heroEntity.GetComponent<Transform>().Scale;
                            heroEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                        }

                        GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                        gm.NewNPC(heroEntity);

                        heroIcon.As<HeroIcon>().HeroBuilt();
                        heroIcon = null;
                        CMConsole.Log("HeroIcon set as null!");
                    }
                }
            }


            //switch (type)
            //{
            //    case AbilityType.SHOOTER:
            //        {

            //            break;
            //        }
            //    case AbilityType.MAGE:
            //        {
            //            //CMConsole.Log("MageNPC Build");
            //            if (mageIcon != null && mageIcon.As<HeroIcon>().trapEntity != null)
            //            {
            //                CMConsole.Log("Condition 1 fufilled");
            //                if (translucentHero == null && built == false)
            //                {
            //                    CMConsole.Log("Should be making the transparent fella here");
            //                    //CMConsole.Log("It shouldnt be here atm");
            //                    translucentHero = CreateGameObject(HeroPrefab);

            //                    // change the opacity here
            //                    translucentHero.GetComponent<Renderer>().SetAlpha(0.3f);
            //                    translucentHero.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
            //                    // translucentHero.GetComponent<Transform>().Depth = depthVal;
            //                    if (IsLeft)
            //                    {
            //                        Vector2 scale = translucentHero.GetComponent<Transform>().Scale;
            //                        translucentHero.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
            //                    }
            //                }
            //            }

            //            else if (mageIcon != null && mageIcon.As<HeroIcon>().trapEntity == null)
            //            {
            //                if (translucentHero != null)
            //                {
            //                    translucentHero.Destroy();
            //                    translucentHero = null;

            //                    // if the player let go when its hovering a build spot
            //                    if (hovering && heroEntity == null)
            //                    {
            //                        // build a trap
            //                        built = true;
            //                        heroEntity = CreateGameObject(HeroPrefab);
            //                        heroEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
            //                        heroEntity.As<HeroAI>().lane = (int)lane;
            //                        heroEntity.As<HeroAI>().active = true;
            //                        heroEntity.As<HeroAI>().type = AbilityType.MAGE;
            //                        if (IsLeft)
            //                        {
            //                            heroEntity.As<HeroAI>().IsLeft = true;
            //                        }
            //                        else
            //                        {
            //                            heroEntity.As<HeroAI>().IsLeft = false;
            //                        }
            //                        Sound.PlaySFX("trap_placement", 0.5f);
            //                        // heroEntity.GetComponent<Transform>().Depth = depthVal;

            //                        if (IsLeft)
            //                        {
            //                            Vector2 scale = heroEntity.GetComponent<Transform>().Scale;
            //                            heroEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
            //                        }

            //                        GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
            //                        gm.NewNPC(heroEntity);

            //                    }
            //                }
            //            }
            //            break;
            //        }
            //}
        }

        public void KillNPC()
        {
            // if it exist
            if (heroEntity != null && heroEntity.mID != 0)
            {
                heroEntity.As<BaseNPC>().KillHero();
                heroEntity = null;
                built = false;
            }
        }


        public void SetHeroType(AbilityType heroType, string heroPrefabName, Entity icon)
        {
            CMConsole.Log("Setting heroType and prefab to" + heroPrefabName);
            type = heroType;
            HeroPrefab = heroPrefabName;
            heroIcon = icon;
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
