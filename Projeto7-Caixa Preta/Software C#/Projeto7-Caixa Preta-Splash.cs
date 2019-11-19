using System;
using System.Windows.Forms;

namespace Parcial2
{
    public partial class Splash : Form
    {
        Form1 principal = new Form1();

        public Splash()
        {
            InitializeComponent();
        }

        int count = 0;

        private void timer1_Tick(object sender, EventArgs e)
        {
            count++;
            if(count == 3)
            {
                this.Hide();
                principal.ShowDialog();
                timer1.Enabled = false;
                this.Close();
            }
        }
    }
}
