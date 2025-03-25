using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Carmicah;

namespace Carmicah
{
    public enum NPCType
    {
        SHOOTER,
        MAGE,
        SPEAR
    }

    public class BaseNPC : Entity
    {
        public string idleAnim = "Shooter_Idle";
        public string shootAnim = "Shooter_Shoot";
        public string manaAnim = "Shooter_Idle";
        public string dissolveAnim = "NPC_Death"; // im using this as death cause i lazy to rename
        public string teleportAnim = "NPC_Teleport";
        public string placeSound = "Shooter_Appear";
        public string shootSound = "Shooter_Shooting";
        public string deathSound = "Shooter_Death";
        public string dissolvePrefab = "Dissolve";

        

        public AbilityType npcType = 0;
        public float shootRate = 1.0f;
        public float shootTime = 1.0f;
        public string projectilePrefab = "Bullet";
        public bool active = false;
        public bool IsLeft = false;
        public int mana = 5;
        public int maxMana = 5;
        
        public int lane;

        public GameManager gameManager;
        public PauseManager pauseManager;
        public Player player;

        protected bool isShooting = false;
        protected bool shot = false;
        protected bool hasAmmo = true;
        protected bool hovering = false;
        protected float animationTime;
        public override void OnCreate()
        {
            CMConsole.Log("Base NPC On Create");
            gameManager = FindEntityWithName("GameManager").As<GameManager>();
            pauseManager = FindEntityWithName("PauseManager").As<PauseManager>();
            player = FindEntityWithName("mainCharacter").As<Player>();

            // base.OnCreate();
        }

        public virtual void ShootProjectile()
        {

        }
        public void HealAmmo()
        {
            Entity dissolveEntity = CreateGameObject(dissolvePrefab);
            if (dissolveEntity != null)
            {
                CMConsole.Log($"dissolve entity pos before {dissolveEntity.Position.x}, {dissolveEntity.Position.y}");
                dissolveEntity.Position = new Vector2(Position.x, Position.y);
                CMConsole.Log($"dissolve entity pos after {dissolveEntity.Position.x}, {dissolveEntity.Position.y}");

            }
            mana = maxMana;
            GetComponent<StateMachine>().SetStateCondition(1);
        }

        public void KillHero()
        {
            GetComponent<StateMachine>().SetStateCondition(4);
        }

        public virtual void GetTarget()
        {

        }

        public virtual void OnStateEnter()
        {

        }

        
        public virtual void OnStateUpdate()
        {

        }

        public override void OnMouseEnter()
        {
            //CMConsole.Log("Hovering!");
            hovering = true;
        }

        public override void OnMouseHover()
        {
            //CMConsole.Log("Hovering!");
            hovering = true;
        }

        public override void OnMouseExit()
        {
            hovering = false;
        }
    }
}
