namespace lab7
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void Triangle_GetHalfPerimeter()
        {
            Triangle triangle = new Triangle();
            triangle.a = triangle.b = triangle.c = 3.0;
            Assert.AreEqual(4.5, Math.Round(triangle.GetHalfPerimeter(), 1));
        }

        [TestMethod]
        public void Triangle_IsValid()
        { 
            Triangle triangle = new Triangle();
            triangle.a = 100.0;
            triangle.b = double.MinValue;
            triangle.c = 3.0;
            Assert.IsFalse(triangle.IsValid());
        }

        [TestMethod]
        public void Triangle_GetSquare()
        {
            //3.897114
            Triangle triangle = new Triangle();
            triangle.a = triangle.b = triangle.c = 3.0;
            Assert.AreEqual(3.897114, Math.Round(triangle.GetSquare(), 6));
        }

        [TestMethod]
        public void Triangle_OperatorIncrement()
        {
            Triangle triangle = new Triangle();
            triangle.a = triangle.b = triangle.c = 3.0;
            triangle++;
            Assert.IsTrue(triangle.a == 4.0 && triangle.b == 4.0 && triangle.c == 4.0);
        }

        [TestMethod]
        public void Triangle_OperatorDecrement()
        {
            Triangle triangle = new Triangle();
            triangle.a = triangle.b = triangle.c = 3.0;
            triangle--;
            Assert.IsTrue(triangle.a == 2.0 && triangle.b == 2.0 && triangle.c == 2.0);
        }

        public void Triangle_OperatorDouble()
        {
            Triangle triangle = new Triangle();
            triangle.a = triangle.b = triangle.c = 3.0;
            Assert.IsTrue(Math.Round((double)triangle, 6) == 3.897114);
        }

        public void Triangle_OperatorBool()
        {
            Triangle triangle = new Triangle();
            triangle.a = triangle.b = triangle.c = 3.0;
            Assert.IsTrue(triangle.IsValid());
        }

        public void MyCollection_Test()
        {
            try
            {
                MyCollection myCollection = new MyCollection(10);
                myCollection[20] = new Triangle();
            }
            catch (Exception ex)
            {
                Assert.IsTrue((ex as ArgumentException != null));
            }
        }
    }
}