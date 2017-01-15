using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace euler579cs2
{
    public class massiveinteger
    {
        int[] digits;

        public void truncate(int size)
        {
            digits = digits.Take(size).ToArray();
        }

        public massiveinteger(long n) : this(n.ToString().Reverse().Select(c => int.Parse(c.ToString())).ToArray())
        {
            
        }

        public massiveinteger() : this(new[] { 0 })
        {
            
        }
        public massiveinteger(int[] result)
        {
            this.digits = result;
        }


        public override string ToString()
        {
            return digits.Reverse().Aggregate(new StringBuilder(), (sb, i) => sb.Append(i.ToString())).ToString().TrimStart('0');
        }

        public static massiveinteger operator +(massiveinteger m1, massiveinteger m2)
        {
            var result = new List<int>();
            int it1 = 0, it2 = 0;

            int remainder = 0;
            bool end1 = it1 == m1.digits.Length, end2 = it2 == m2.digits.Length;
            for (bool carryon = true; carryon;)
            {
                int d1 = end1 ? 0 : m1.digits[it1];
                int d2 = end2 ? 0 : m2.digits[it2];
                int tot = d1 + d2 + remainder;
                if (tot == 0 && end1 && end2) carryon = false;
                else
                {
                    int item = tot % 10;
                    remainder = (tot - item) / 10;
                    result.Add(item);
                    end1 = end1 || (++it1 >= m1.digits.Length);
                    end2 = end2 || (++it2 >= m2.digits.Length);
                }
            }
            return new massiveinteger(result.ToArray());
        }

        public static massiveinteger operator *(massiveinteger m1, massiveinteger m2)
        {
            var total = new massiveinteger();
            var current = m1.digits.ToList();
            foreach (var digit in m2.digits)
            {
                var currentval = new massiveinteger(current.ToArray());
                for (int i = 0; i < digit; i++)
                {
                    total += currentval;
                }
                current.Insert(0, 0);
            }
            return total;
        }
    }
    
}
