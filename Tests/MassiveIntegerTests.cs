using System;
using euler579cs2;
using NUnit.Framework;

namespace Tests
{
    [TestFixture]
    public class MassiveIntegerTests
    {
        //[Test]
        public void GenerateSomeNumbers()
        {
            var r = new Random();
            for (int i = 0; i < 20; i++)
            {
                Console.Out.WriteLine($"[TestCase({r.Next(1000000, 9999999)}, {r.Next(1000000, 9999999)})]");
            }
        }

        [TestCase(19L, 12L)]
        [TestCase(4609671L, 7823421L)]
        [TestCase(8229066L, 7041511L)]
        [TestCase(4789039L, 1712097L)]
        [TestCase(3919986L, 9227515L)]
        [TestCase(2673504L, 1905644L)]
        [TestCase(7164300L, 8545216L)]
        [TestCase(1769495L, 3030738L)]
        [TestCase(3540290L, 9941264L)]
        [TestCase(2805123L, 8148203L)]
        [TestCase(3504069L, 8345731L)]
        [TestCase(5661621L, 3910497L)]
        [TestCase(2513357L, 6889317L)]
        [TestCase(3852715L, 9642277L)]
        [TestCase(7367496L, 6057148L)]
        [TestCase(4117677L, 4281059L)]
        [TestCase(4657123L, 9052134L)]
        [TestCase(8724140L, 8742189L)]
        [TestCase(3070798L, 8427402L)]
        [TestCase(4345884L, 7562325L)]
        [TestCase(3896217L, 4424580L)]
        public void TestAddition(long n1, long n2)
        {
            var m1 = new massiveinteger(n1);
            var m2 = new massiveinteger(n2);
            var m = m1 + m2;
            Assert.AreEqual(m.ToString(), (n1 + n2).ToString());
        }

        [TestCase(4609671L, 7823421L)]
        [TestCase(8229066L, 7041511L)]
        [TestCase(4789039L, 1712097L)]
        [TestCase(3919986L, 9227515L)]
        [TestCase(2673504L, 1905644L)]
        [TestCase(7164300L, 8545216L)]
        [TestCase(1769495L, 3030738L)]
        [TestCase(3540290L, 9941264L)]
        [TestCase(2805123L, 8148203L)]
        [TestCase(3504069L, 8345731L)]
        [TestCase(5661621L, 3910497L)]
        [TestCase(2513357L, 6889317L)]
        [TestCase(3852715L, 9642277L)]
        [TestCase(7367496L, 6057148L)]
        [TestCase(4117677L, 4281059L)]
        [TestCase(4657123L, 9052134L)]
        [TestCase(8724140L, 8742189L)]
        [TestCase(3070798L, 8427402L)]
        [TestCase(4345884L, 7562325L)]
        [TestCase(3896217L, 4424580L)]
        public void TestMultiplication(long n1, long n2)
        {
            var m1 = new massiveinteger(n1);
            var m2 = new massiveinteger(n2);
            var m = m1 * m2;
            Assert.AreEqual(m.ToString(), (n1 * n2).ToString());
        }

        
        
    }
}
