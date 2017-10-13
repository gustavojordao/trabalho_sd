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
public class ClienteRecebeMensagem implements Runnable {

    @Override
    public void run() {
        while (true) {
            Mensagem m = Main.node.getAntecessor().receber();
            Date data = new Date();

            if (m != null) {

                System.out.println(data.toString() + " - " + m.toString());

                switch (m.getCodigo()) {
                    case 2:
                        Main.node.getAntecessor().enviar(Mensagem.mensagemRespostaSolicitacaoPorta(Main.node.getPorta_suc()));
                        break;
                }
            }
        }
    }

}
