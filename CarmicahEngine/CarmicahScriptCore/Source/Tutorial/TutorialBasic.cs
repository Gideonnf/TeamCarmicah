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
        public bool justHovered = false;
        public bool clicked     = false;

        bool GetIsClick()
        {
            bool ret = clicked;
            clicked = false;
            return clicked;
        }

        bool GetEnterHover()
        {
            bool ret = justHovered;
            justHovered = false;
            return ret;
        }

        bool GetHover()
        {
            return hovering;
        }

        void OnClick()
        {
            clicked = true;
        }

        void OnMouseHover()
        {
            if (!hovering)
            {
                justHovered = true;
                hovering = true;
            }
        }

        void OnMouseExit()
        {
            hovering = false;
            justHovered = false;
        }

    }
}