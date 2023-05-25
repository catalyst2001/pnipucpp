using lab3;

namespace lab3_test
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestMethod1()
        {
            Assert.AreEqual(true, Math.Round(Math.Abs(Functions.ComputeSN(0.1, 10.0) - 0.00499)) < float.Epsilon);
            Assert.AreEqual(true, Math.Round(Math.Abs(Functions.ComputeSE(0.1, 0.0001) - 0.00499)) < float.Epsilon);
            Assert.AreEqual(true, Math.Round(Math.Abs(Functions.ComputeY(0.1) - 0.00499)) < float.Epsilon);
        }
    }
}