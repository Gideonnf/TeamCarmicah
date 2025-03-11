using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Text;
using System.Threading.Tasks;

namespace CarmicahScript
{
    public class TutorialBasic : Entity
    {
        bool hovering           = false;
        bool justHovered = false;
        bool clicked     = false;

        public bool GetIsClick()
        {
            bool ret = clicked;
            clicked = false;
            return clicked;
        }

        public bool GetEnterHover()
        {
            bool ret = justHovered;
            justHovered = false;
            return ret;
        }

        public bool GetHover()
        {
            return hovering;
        }

        public override void OnClick()
        {
            clicked = true;
        }

        public override void OnMouseHover()
        {
            if (!hovering)
            {
                justHovered = true;
                hovering = true;
            }
        }

        public override void OnMouseExit()
        {
            hovering = false;
            justHovered = false;
        }

    }
}