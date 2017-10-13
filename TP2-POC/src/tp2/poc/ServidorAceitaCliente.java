/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tp2.poc;

/**
 *
 * @author 123437
 */
public class ServidorAceitaCliente implements Runnable {

    @Override
    public void run() {
        Main.node.getSucessor().aceitar();
        
        Main.t_srv_recv.start();
        
        if(Main.node.getAntecessor() == null){
            Main.node.getSucessor().enviar(Mensagem.mensagemSolicitacaoPorta());
        }
        
    }
    
}
