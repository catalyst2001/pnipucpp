using Lab1n;

namespace lab1_test
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestMethod1()
        {
            var a = Lab1.point_in_circle(0, 0, 1, 0, 0);
            Assert.AreEqual(true, a);

        }
    }
}