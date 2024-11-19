FROM node:20.18.0

WORKDIR /app

COPY package.json package-lock.json ./

RUN npm ci --omit=dev

RUN npm rebuild bcrypt --build-from-source

COPY . .

EXPOSE 8080

CMD ["node", "app.js"]
