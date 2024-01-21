using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace lab10
{
    public class Factory : Organization
    {
        protected int done_plan;
        protected int total_plan;

        public Factory() {
            total_plan = 0;
            done_plan = 0;
        }

        public Factory(string name, string address, string worktime, int plan, int done_plan) : base(name, address, worktime)
        {
            this.total_plan = plan;
            this.done_plan = done_plan;
        }

        public int getPlan() { return total_plan; }
        public void setPlan(int plan) { this.total_plan = plan; }
        public int getDonePlan() {  return done_plan; }
        public void setDonePlan(int plan) { done_plan = plan; }

        public new string GetFullInfo()
        {
            return $"Factory: {name}, {address}, {worktime}. (Done|Total) All({done_plan}|{total_plan})";
        }

        public new void AutoFill()
        {
            FillDefault("Factory");
        }

        public override string ToString()
        {
            return GetFullInfo();
        }
    }
}
