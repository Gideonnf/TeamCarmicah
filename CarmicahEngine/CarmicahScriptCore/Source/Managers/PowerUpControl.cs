using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class PowerUpControl : Entity
    {
        public string TrapPrefab = "TrapBall";
        public string ShooterPrefab = "ShooterBall";
        public string MagePrefab = "MageBall";
        public string HoneyPrefab = "HoneyBall";

        private Dictionary<IconType, float> weights = new Dictionary<IconType, float>()
        {
            {IconType.CANDY_ICON, 0.4f },
            {IconType.SHOOTER_ICON, 0.1f },
            {IconType.HONEY_ICON, 0.4f },
            {IconType.MAGE_ICON, 0.1f }
        };

        public IconType GetRandomTrap()
        {
            float totalWeight = weights.Values.Sum();
            float random = CMRand.Range(0.0f, totalWeight);
            float cumulative = 0.0f;

            foreach(var trap in weights)
            {
                cumulative += random;
                if (random <= cumulative)
                {
                    return trap.Key;
                }
            }

            return IconType.CANDY_ICON;
        }

        public override void OnUpdate(float dt)
        {
            
        }

    }
}
