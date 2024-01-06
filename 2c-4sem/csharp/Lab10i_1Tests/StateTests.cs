using Microsoft.VisualStudio.TestTools.UnitTesting;
using Lab10_1;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab10_1.Tests
{
    [TestClass()]
    public class StateTests
    {
        [TestMethod()]
        public void StateTest()
        {
            var state = new State("1", "2");
            Assert.AreEqual("1", state.GetName());
            Assert.AreEqual("2", state.GetGovernmentForm());

            state.SetName("3");
            state.SetGovernmentForm("4");
            Assert.AreEqual("3", state.GetName());
            Assert.AreEqual("4", state.GetGovernmentForm());
        }

        [TestMethod()]
        public void RandomInitTest()
        {
            var state = new State("Страна", "Республика");
            state.RandomInit();
            Assert.IsTrue(((IRandomInit)state).random > 0);
        }

        [TestMethod()]
        public void ShallowCopyTest()
        {
            var state = new State("Страна", "Республика");
            var copy = state.ShallowCopy();
            Assert.AreEqual(state.GetName(), copy.GetName());
        }

        [TestMethod()]
        public void CloneTest()
        {
            var state = new State("Страна", "Республика");
            var copy = (State)state.Clone();
            Assert.AreEqual(state.GetName(), copy.GetName());
        }

        [TestMethod()]
        public void CompareToTest()
        {
            var state1 = new State("Страна", "Республика");
            var state2 = new State("Страна", "Республика");
            state1.RandomInit();
            state2.RandomInit();
            var rand1 = ((IRandomInit)state1).random;
            var rand2 = ((IRandomInit)state2).random;
            int expected = 0;
            if (rand1 > rand2)
                expected = 1;
            if (rand1 < rand2)
                expected = -1;
            int actual = state1.Compare(state1, state2);
            Assert.AreEqual(expected, actual);
        }

        [TestMethod()]
        public void CompareTest()
        {
            var state1 = new State("Страна", "Республика");
            var state2 = new State("Страна", "Республика");
            state1.RandomInit();
            state2.RandomInit();
            var rand1 = ((IRandomInit)state1).random;
            var rand2 = ((IRandomInit)state2).random;
            int expected = 0;
            if (rand1 > rand2)
                expected = 1;
            if (rand1 < rand2)
                expected = -1;
            int actual = state1.CompareTo(state2);
            Assert.AreEqual(expected, actual);
        }
    }
}