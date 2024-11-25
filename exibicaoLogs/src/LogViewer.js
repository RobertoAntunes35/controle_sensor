import React, { useEffect, useState, useRef } from "react";
import SockJS from "sockjs-client";
import { Stomp, Client as StompClient } from "@stomp/stompjs";

import { v4 as uuidv4 } from 'uuid';
const LogViewer = ( {onAuthTokenReceived} ) => {
    const [logs, setLogs] = useState([]);
    const logContainerRef = useRef(null); // Ref para o container de logs


    useEffect(() => {
        console.log("useEffect foi chamado!");
        const client = Stomp.over(function() {
            return new WebSocket('ws://192.168.0.111:15674/ws');
        });

        const onConnect = function() {
            console.log('Conectado');


            client.subscribe('/exchange/login/auth', (message) => {
                if (message.body) {
                    try {
                        // Parse da mensagem JSON
                        const logData = JSON.parse(message.body);

                        // Criando uma estrutura de log com base nos dados recebidos
                        const token = logData.token;
                        onAuthTokenReceived(token);

                        console.log(message)
                    } catch (error) {
                        console.error("Erro ao parsear a mensagem:", error);
                    }
                    console.log(message.body);
                    message.ack()
                } else {
                    console.log("SEM MENSAGEM");
                }
            })

            client.subscribe('/exchange/logs/info', (message) => {
                if (message.body) {
                    try {
                        // Parse da mensagem JSON
                        const logData = JSON.parse(message.body);

                        // Criando uma estrutura de log com base nos dados recebidos
                        const logId = logData.id;
                        const api = logData.api;
                        const logMessage = logData.message;
                        const status = logData.status;
                        const time = logData.time;

                        setLogs((prevLogs) => [
                            ...prevLogs,
                            { id: logId, api, message: logMessage, status, time }
                        ]);
                    } catch (error) {
                        console.error("Erro ao parsear a mensagem:", error);
                    }
                    console.log(message.body);
                    message.ack()
                } else {
                    console.log("SEM MENSAGEM");
                }
            });

            client.subscribe('/exchange/logs/error', (message) => {
                if (message.body) {
                    try {
                        // Parse da mensagem JSON
                        const logData = JSON.parse(message.body);

                        // Criando uma estrutura de log com base nos dados recebidos
                        const logId = logData.id;
                        const api = logData.api;
                        const logMessage = logData.message;
                        const status = logData.status;
                        const time = logData.time;

                        setLogs((prevLogs) => [
                            ...prevLogs,
                            { id: logId, api, message: logMessage, status, time }
                        ]);
                    } catch (error) {
                        console.error("Erro ao parsear a mensagem:", error);
                    }
                    console.log(message.body);
                    message.ack()
                } else {
                    console.log("SEM MENSAGEM");
                }
            });
        };

        const onError = function() {
            console.log('Erro');
        };

        client.connect('guest', 'guest', onConnect, onError);

        return () => {
            // Cleanup quando o componente for desmontado
            client.disconnect(() => {
                console.log("Desconectado");
            });
        };
    }, []);

    // Função para definir a cor do card baseado no status
    const getCardColor = (status) => {
        if (status === 200) {
            return "#28a745";
        }
        else if (status === "NAO SE APLICA") {
            return "#6c757d";
        }
        else {
            return "#dc3545";
        }
    };

    // Função para rolar automaticamente para o final do container
    useEffect(() => {
        if (logContainerRef.current) {
            logContainerRef.current.scrollTop = logContainerRef.current.scrollHeight;
        }
    }, [logs]); // Sempre que 'logs' mudar, o scroll é atualizado

    return (
        <div style={{ padding: "20px", backgroundColor: "#121212", minHeight: "100vh" }}>
            <h1 style={{ color: "white", textAlign: "center" }}>Logs em Tempo Real</h1>
            <div
                ref={logContainerRef} // Atribuindo o ref ao container
                style={{
                    border: "1px solid #ccc",
                    padding: "10px",
                    height: "100vh",
                    overflowY: "scroll",
                    backgroundColor: "black", // Fundo preto
                    color: "white", // Mensagens brancas
                    fontFamily: "monospace", // Estilo de fonte para logs
                    transition: "all 0.3s ease", // Animação suave de transição
                }}
            >
                {logs.map((log) => (
                    <div
                        key={log.id}
                        style={{
                            backgroundColor: getCardColor(log.status), // cor dinâmica com base no status
                            padding: "15px",
                            marginBottom: "10px",
                            borderRadius: "8px",
                            border: "1px solid #444",
                            boxShadow: "0px 4px 6px rgba(0, 0, 0, 0.2)",
                            transition: "transform 0.3s ease", // Animação de entrada
                        }}
                    >
                        <div style={{ marginBottom: "8px" }}>
                            <strong>ID:</strong> <span>{log.id}</span>
                        </div>
                        <div style={{ marginBottom: "8px" }}>
                            <strong>API:</strong> <span>{log.api}</span>
                        </div>
                        <div style={{ marginBottom: "8px" }}>
                            <strong>Status:</strong> <span>{log.status}</span>
                        </div>
                        <div style={{ marginBottom: "8px" }}>
                            <strong>Mensagem:</strong>
                            <pre>{log.message}</pre>
                        </div>
                        <div>
                            <strong>Hora:</strong> <span>{log.time}</span>
                        </div>
                    </div>
                ))}
            </div>
        </div>
    );
};

export default LogViewer;
