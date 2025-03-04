using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public enum AbilityType
    {
        CANDY_CONE,
        HONEY,
        SHOOTER,
        MAGE,
        SPEAR
    }
    public class BaseIcon : Entity
    {
        public string WaveSystemObject = "Something";
        public string fakeTrapPrefab = "";
        public string actualTrapPrefab = "";
        public Entity trapEntity;
        public bool flipped = false;
        public bool hovering = false;

        public AbilityType type;
        public Entity waveSystem;

        public override void OnCreate()
        {
            waveSystem = FindEntityWithName(WaveSystemObject);
        }

        public void NotifyPowerBar()
        {
            Entity powerBar = FindEntityWithName("UIBar");
            powerBar.As<AbilityBar>().IconRemoved(this);
        }

    }
}
