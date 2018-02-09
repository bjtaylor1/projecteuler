using System;
using System.Collections.Concurrent;
using Mpir.NET;

namespace _0178
{
    class Program
    {
        static readonly ConcurrentDictionary<(int, int, string), mpz_t> cache = new ConcurrentDictionary<(int, int, string), mpz_t>();
        static mpz_t GetPandigitalStepNumbersCount(int totalDigits, int lastDigit, string digitRequirements)
        {
            return cache.GetOrAdd((totalDigits, lastDigit, digitRequirements), MakePandigitalStepNumbersCount);
        }

        static mpz_t MakePandigitalStepNumbersCount((int totalDigits, int lastDigit, string digitRequirements) param)
        {
            if (param.totalDigits == 1 && (param.lastDigit.ToString() == param.digitRequirements || string.Empty == param.digitRequirements))
                return param.lastDigit > 0 ? 1 : 0;

            if (param.totalDigits <= 0) return 0;

            var result = new mpz_t();
            string newDigitRequirements = param.digitRequirements.Replace($"{param.lastDigit}", "");
            if (param.lastDigit > 0) result += GetPandigitalStepNumbersCount(param.totalDigits - 1, param.lastDigit - 1, newDigitRequirements);
            if (param.lastDigit < 9) result += GetPandigitalStepNumbersCount(param.totalDigits - 1, param.lastDigit + 1, newDigitRequirements);
            int intResult = (int)result;
            return result;
        }

        static void Main(string[] args)
        {
            mpz_t result = 0;
            for (int totalDigits = 0; totalDigits <= 40; totalDigits++)
            {
                for (int lastDigit = 0; lastDigit <= 9; lastDigit++)
                {
                    result += GetPandigitalStepNumbersCount(totalDigits, lastDigit, "0123456789");
                }
            }
            Console.WriteLine(result);
        }
    }
}
