using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace lab12k
{
    /* судостроительная */
    public class ShipbuildingCompany : Factory
    {
        protected int barges_total;
        protected int barges_done;
        protected int river_barges_total;
        protected int river_barges_done;

        public ShipbuildingCompany() : base("", "", "", 0, 0)
        {
            barges_total = 0;
            barges_done = 0;
            river_barges_total = 0;
            river_barges_done = 0;
            subobject = new Organization("", "", "");
        }

        public ShipbuildingCompany(string name, string address, string worktime, int barges_total, int barges_done, int rbarges_total, int rbarges_done)
            : base(name, address, worktime, barges_total + rbarges_total, barges_done + rbarges_done)
        {
            this.barges_total = barges_total;
            this.barges_done = barges_done;
            river_barges_total = rbarges_total;
            river_barges_done = rbarges_done;
            subobject = new Organization(name, address, worktime);
        }

        public int GetBargesTotal() { return barges_total; }
        public int GetBargesDone() {  return barges_done; }
        public int GetRiverbargesTotal() { return river_barges_total; }
        public int GetRiverbargesDone() { return river_barges_done; }

        public new string GetFullInfo()
        { 
           return $"ShipbuildingCompany: {name}, {address}, {worktime} (Total|Done) All({total_plan}|{done_plan}) barges({barges_total}|{barges_done}) riverbarges({river_barges_total}|{river_barges_done})";
        }

        public override void FillInfo()
        {
            base.FillInfo();
            barges_total = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "Enter barges_total");
            barges_done = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "Enter barges_done");
            river_barges_total = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "Enter river_barges_total");
            river_barges_done = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "Enter river_barges_done");
        }
    }
}
