using System;
using System.Windows.Forms;

namespace Blink_LED
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                serialPort1.Open();//Objeto serialPort1 foi criado a partir de um componente
                /*Por padrão:
                 * PortName = "COM4"
                 * BaudRate = 9600
                */
            }
            catch
            {
                MessageBox.Show("Erro ao fazer conexão", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            serialPort1.WriteLine(trackBar1.Value.ToString());
        }
    }
}