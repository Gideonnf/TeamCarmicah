using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class TutorialBasic : Entity
    {
        bool hovering    = false;
        bool justHovered = false;
        bool justUnhovered = false;

        public bool GetEnterHover()
        {
            bool ret = justHovered;
            justHovered = false;
            return ret;
        }

        public bool GetExitHover()
        {
            bool ret = justUnhovered;
            justUnhovered = false;
            return ret;
        }

        public bool GetHover()
        {
            return hovering;
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
            justUnhovered = true;
        }

    }
}