using System;
using System.Windows.Forms;

namespace IR_LED_CONTROL
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        String Rxstring;

        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort1.PortName = "COM4";
            serialPort1.BaudRate = 9600;
            try
            {
                serialPort1.Open();//Criado a partir de um componente
            }
            catch
            {
                MessageBox.Show("Erro ao conectar-se!", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                Application.Exit();
            }
        }

        private void TrataDados(object sender, EventArgs e)
        {
            LbCod.Text = Rxstring;            
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {           
            Rxstring = serialPort1.ReadLine();
            Invoke(new EventHandler(TrataDados));
        }
    }
}
