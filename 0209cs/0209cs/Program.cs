using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using EqualityComparers;

namespace _0209cs
{
    class Program
    {
        static void Main(string[] args)
        {
            long res = 0;
            int[] bitpos1 = new[] { 0, 1, 2, 3, 4, 5, 6, 7 };
            int[] bitpos2 = new[] { 0, 2, 4, 7, 1, 3, 5, 6 };
            for(long l = 0; l < 256; l++)
            {
                long[] bits1 = bitpos1.Select(b => (l >> b) & 1).ToArray();
                long[] bits2 = bitpos2.Select(b => (l >> b) & 1).ToArray();
                bool isMatch = true;
                for (long i = 0; i < bitpos1.Length; i++)
                {
                    if ((bits1[i] & bits2[i]) != 0)
                    {
                        isMatch = false;
                        break;
                    }
                }
                if (isMatch) res++;
            }
            Console.WriteLine(res);
        }
    }

    public class Exclusion
    {
        public string Exclude { get; set; }
        public string Reinclude { get; set; }

        public override bool Equals(object obj)
        {
            return obj is Exclusion exclusion &&
                   Exclude == exclusion.Exclude &&
                   Reinclude == exclusion.Reinclude;
        }

        public override int GetHashCode()
        {
            var hashCode = 119986229;
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(Exclude);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(Reinclude);
            return hashCode;
        }

        public override string ToString()
        {
            return $"{Exclude} - {Reinclude}";
        }
    }
}
