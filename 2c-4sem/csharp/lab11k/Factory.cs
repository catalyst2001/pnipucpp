using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab11k
{
    public class Factory : Organization
    {
        protected int done_plan;
        protected int total_plan;

        public Factory() : base("", "", "") {
            done_plan = 0;
            total_plan = 0;
            subobject = new Organization("", "", "");
        }
        public Factory(string name, string address, string worktime, int plan, int done) : base(name, address, worktime)
        {
            total_plan = plan;
            done_plan = done;
            subobject = new Organization(name, address, worktime);
        }

        public int getPlan() { return total_plan; }
        public void setPlan(int plan) { total_plan = plan; }
        public int getDonePlan() {  return done_plan; }
        public void setDonePlan(int plan) { done_plan = plan; }

        public new string GetFullInfo()
        {
            return $"Factory: {name}, {address}, {worktime}. (Done|Total) All({done_plan}|{total_plan})";
        }

        public override void FillInfo() 
        {
            base.FillInfo();
            done_plan = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "Enter done plan");
            total_plan = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "Enter total plan");
        }
    }
}
