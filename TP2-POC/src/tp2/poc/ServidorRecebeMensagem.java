/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tp2.poc;

import java.util.Date;

/**
 *
 * @author 123437
 */
public class ServidorRecebeMensagem implements Runnable {

    @Override
    public void run() {
        while (true) {
            Mensagem m = Main.node.getSucessor().receber();
            System.out.println("Ok");
            Date data = new Date();

            if (m != null) {

                System.out.println(data.toString() + " - " + m.toString());

                String[] partes = null;
                int porta = 0;

                switch (m.getCodigo()) {
                    case 3:
                        porta = Integer.parseInt(m.getTexto());

                        Main.node.setAntecessor(new Cliente(Main.node.getSucessor().getIpCliente(), porta));
                        Main.node.getAntecessor().conectar();
                        
                        Main.t_cli_recv.start();

                        break;
                }
            }
        }
    }

}
