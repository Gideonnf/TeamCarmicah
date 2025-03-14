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
        //string soundFile;
        Entity translucentHero;
        public Entity heroEntity;
        //Entity shooterIcon;
        //Entity mageIcon;
        AbilityType type;
        bool hovering = false;
        bool built = false;
        Entity heroIcon;
        public float xOffset = 1.5f;
        public float yOffset = 1.0f;

        public override void OnCreate()
        {
            //shooterIcon = FindEntityWithName("ShooterIcon");
            //mageIcon = FindEntityWithName("MageIcon");
        }

        public override void OnUpdate(float dt)
        {
            //CMConsole.Log($"Hovering {hovering}");
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
                    translucentHero.GetComponent<Transform>().Position = new Vector2(Position.x + xOffset, Position.y + yOffset);
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
                        heroEntity.GetComponent<Transform>().Position = new Vector2(Position.x + xOffset, Position.y + yOffset);
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

                        PlacementSound(HeroPrefab);
                        
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
        }

        public void PlacementSound(string heroPrefabName)
        {
            
            if (HeroPrefab == "ShooterNPC")
            {
                
                heroEntity.As<ShooterNPC>().PlayVoiceOver();
            }
            else if (HeroPrefab == "MageNPC")
            {
                
                heroEntity.As<MageNPC>().PlayVoiceOver();


            }
            else if(HeroPrefab == "SpearNPC")
            {
                
                heroEntity.As<SpearNPC>().PlayVoiceOver();
            }
            
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

            xOffset = heroIcon.As<HeroIcon>().xOffset;
            yOffset = heroIcon.As<HeroIcon>().yOffset;
        }

        public override void OnMouseEnter()
        {
            hovering = true;
        }

        public override void OnMouseHover()
        {
            hovering = true;
        }

        public override void OnMouseExit()
        {
            hovering = false;
        }
    }
}
