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
                        ChangeAnim("StrawberryCake_Squish");
                    break;
                }
                case CakeTypes.MATCHA_CAKE:
                {
                        ChangeAnim("MatchaCake_Squish");
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
