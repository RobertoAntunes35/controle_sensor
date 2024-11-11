
import { connect } from 'amqplib';
import { v4 as uuidv4 } from 'uuid';

export default class RabbitmqServer {
    conn;
    channel;
    exchange_log;
    exchange_login;
    uri;
    queueError;
    queueInfo;
    queueAuth; 


    constructor(uri, exchange_log = 'logs', exchange_login = 'login', queueError = 'error_logs', queueInfo = 'info_logs', queueAuth = 'auth_log') {
        this.uri = uri;
        this.exchange_log = exchange_log;
        this.queueError = queueError;
        this.queueInfo = queueInfo;
        this.exchange_login = exchange_login;
        this.queueAuth = queueAuth;
    }

    async start() {
        this.conn = await connect(this.uri);
        this.channel = await this.conn.createChannel();

        // declara as exchange
        await this.channel.assertExchange(this.exchange_log, 'direct', { durable: true });
        await this.channel.assertExchange(this.exchange_login, 'direct', { durable: true });

        // declara as filas 
        await this.channel.assertQueue(this.queueError, {durable: true});
        await this.channel.assertQueue(this.queueInfo, {durable: true});
        await this.channel.assertQueue(this.queueAuth, {durable: true});

        // vincula as filas as routing key
        await this.channel.bindQueue(this.queueError, this.exchange_log, 'error');
        await this.channel.bindQueue(this.queueInfo, this.exchange_log, 'info');
        await this.channel.bindQueue(this.queueAuth, this.exchange_login, 'auth');
    }

    async publishInQueueLogs(routing_key, message, status) {
        const time = new Date();
        let full_message = {
            id: uuidv4(),
            api: "auth_user",
            message: message,
            status: status,
            time: time
        }
        return await this.channel.publish(this.exchange_log, routing_key, Buffer.from(JSON.stringify(full_message)))
    }

    async publishInQueueAuth(routing_key, email, token) {
        const time = new Date();
        let full_message = {
            id: uuidv4(),
            api: "auth_user",
            email: email,
            token: token,
            time: time
        }
        return await this.channel.publish(this.exchange_login, routing_key, Buffer.from(JSON.stringify(full_message)))
    }

    async consume(queue, callback) {
        return await this.channel.consume(queue, (message) => {
            if (message !== null) {
                callback(message.content.toString());
                this.channel.ack(message);
            }
        });
    }
}