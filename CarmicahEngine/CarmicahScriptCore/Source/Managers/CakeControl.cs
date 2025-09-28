using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public enum CakeTypes
    {
        STRAWBERRY_CAKE,
        MATCHA_CAKE,
        RAINBOW_CAKE,
        FRUIT_CAKE,
        TOTAL_CAKES
    }

    public class CakeControl : Entity
    {
        // default to strawberry cake
        CakeTypes cakeType = CakeTypes.STRAWBERRY_CAKE;
        //public string animSquish;

        public override void OnCreate()
        {
          // randomise cake type on creation
          
        }

        public void PlaySquishAnim()
        {
            switch(cakeType)
            {
                case CakeTypes.STRAWBERRY_CAKE:
                {
                        ChangeAnim("CakeMoon_Squish");
                    break;
                }
                case CakeTypes.MATCHA_CAKE:
                {
                        ChangeAnim("CakeMoon_Squish");
                    break;
                }
                case CakeTypes.RAINBOW_CAKE:
                {
                        ChangeAnim("CakeMoon_Squish");
                        break;
                }
                case CakeTypes.FRUIT_CAKE:
                {
                        ChangeAnim("CakeMoon_Squish");
                        break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
}
