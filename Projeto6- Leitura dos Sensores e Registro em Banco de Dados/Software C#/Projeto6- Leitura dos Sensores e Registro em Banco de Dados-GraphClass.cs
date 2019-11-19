namespace Projeto_Parcial_1
{
    class GraphClass
    {
        public static string[] LdrValues = new string[50];
        public static string[] PotValues = new string[50];
        public static int LdrIndex=0;
        public static int PotIndex=0;

        public int ldrIndex
        {
            get { return LdrIndex; }
            set { LdrIndex = value; }
        }

        public int potIndex
        {
            get { return PotIndex; }
            set { PotIndex = value; }
        }

        public void setldrValue(int i,string x)
        {
            LdrValues[i] = x;
        }

        public string getldrValue(int i)
        {
            return LdrValues[i];
        }

        public void setpotValue(int i, string x)
        {
            PotValues[i] = x;
        }

        public string getpotValue(int i)
        {
            return PotValues[i];
        }
    }
}
