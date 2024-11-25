import React, { useState } from "react";
import { Button, Container, Grid, Box, TextField, Typography } from "@mui/material";
import LogViewer from "./LogViewer";
import SensorControl from "./SensorControl";

const App = () => {
  const [authToken, setAuthToken] = useState(""); // Estado para armazenar o token
  const [selectedView, setSelectedView] = useState("sensor1"); // Estado para controlar qual componente será exibido
  const [email, setEmail] = useState(""); // Estado para o email
  const [password, setPassword] = useState(""); // Estado para a senha de autenticação
  const [customPassword, setCustomPassword] = useState(""); // Estado para a senha customizada
  const [authError, setAuthError] = useState(""); // Estado para exibir erros de autenticação
  const [passwordResponse, setPasswordResponse] = useState(""); // Estado para resposta da senha enviada
  const [token, setToken] = useState(""); // Estado para resposta da senha enviada


  // Função chamada pelo LogViewer ao receber o token
  const handleAuthTokenReceived = (token) => {
    console.log("Token recebido no App:", token);
    setAuthToken(token); // Armazena o token no estado
  };

  const handleButtonClick = (view) => {
    setSelectedView(view); // Muda a visualização com base no botão clicado
  };

  // Função para autenticação
  const handleAuthSubmit = async () => {
    setAuthError(""); // Reseta o erro
    try {
      const response = await fetch("http://localhost:8080/api/v1/user/auth", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ email, password }),
      });
      if (response.ok) {
        const data = await response.json();
        setAuthToken(data.token); // Armazena o token recebido
        console.log("Token recebido:", data.accessToken);
        alert("Autenticação bem-sucedida!");
        alert(data.accessToken);
      } else {
        const errorData = await response.json();
        setAuthError(errorData.message || "Erro ao autenticar.");
      }
    } catch (error) {
      console.error("Erro na autenticação:", error);
      setAuthError("Erro na conexão com o servidor.");
    }
  };

  // Função para enviar a senha para a segunda URL
  const handlePasswordSubmit = async () => {
    setPasswordResponse(""); // Reseta a resposta
    try {
      const response = await fetch("http://192.168.0.111:5000/api/v1/insert-password", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          Authorization: `Bearer ${token}`

        },
        body: JSON.stringify({ password: customPassword }),
      });
      if (response.ok) {
        const data = await response.json();
        setPasswordResponse("Senha enviada com sucesso!");
        console.log("Resposta do servidor:", data);
        alert("Senha enviada com sucesso!");
      } else {
        setPasswordResponse("Erro ao enviar senha.");
      }
    } catch (error) {
      console.error("Erro ao enviar senha:", error);
      setPasswordResponse("Erro na conexão com o servidor.");
    }
  };

  return (
    <Container maxWidth="lg" style={{ paddingTop: "20px" }}>
      {/* Cabeçalho e Botões de Navegação */}
      <Box display="flex" justifyContent="center" mb={4}>
        <Button
          variant={selectedView === "logs" ? "contained" : "outlined"}
          color="primary"
          onClick={() => handleButtonClick("logs")}
          style={{ marginRight: "10px" }}
        >
          Logs
        </Button>
        <Button
          variant={selectedView === "sensores" ? "contained" : "outlined"}
          color="secondary"
          onClick={() => handleButtonClick("sensores")}
          style={{ marginRight: "10px" }}
        >
          Sensores
        </Button>
        <Button
          variant={selectedView === "auth" ? "contained" : "outlined"}
          color="primary"
          onClick={() => handleButtonClick("auth")}
          style={{ marginRight: "10px" }}
        >
          Autenticar
        </Button>
        <Button
          variant={selectedView === "insert-password" ? "contained" : "outlined"}
          color="secondary"
          onClick={() => handleButtonClick("insert-password")}
        >
          Enviar Senha
        </Button>
      </Box>

      {/* Exibição Condicional dos Componentes */}
      {selectedView === "logs" && (
        <LogViewer onAuthTokenReceived={handleAuthTokenReceived} />
      )}

      {selectedView === "sensores" && <SensorControl authToken={authToken} />}

      {selectedView === "auth" && (
        <Box display="flex" flexDirection="column" alignItems="center">
          <Typography variant="h5" gutterBottom>
            Autenticação
          </Typography>
          <TextField
            label="Email"
            type="email"
            value={email}
            onChange={(e) => setEmail(e.target.value)}
            style={{ marginBottom: "10px", width: "300px" }}
          />
          <TextField
            label="Senha"
            type="password"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            style={{ marginBottom: "10px", width: "300px" }}
          />
          <Button
            variant="contained"
            color="primary"
            onClick={handleAuthSubmit}
          >
            Enviar
          </Button>
          {authError && (
            <Typography
              color="error"
              style={{ marginTop: "10px", textAlign: "center" }}
            >
              {authError}
            </Typography>
          )}
        </Box>
      )}

      {selectedView === "insert-password" && (
        <Box display="flex" flexDirection="column" alignItems="center">
          <Typography variant="h5" gutterBottom>
            Enviar Senha
          </Typography>
          <TextField
            label="Token"
            value={token}
            onChange={(e) => setToken(e.target.value)}
            style={{ marginBottom: "10px", width: "300px" }}
          />
          <TextField
            label="Senha"
            type="number"
            value={customPassword}
            onChange={(e) => setCustomPassword(e.target.value)}
            style={{ marginBottom: "10px", width: "300px" }}
          />
          <Button
            variant="contained"
            color="secondary"
            onClick={handlePasswordSubmit}
          >
            Enviar
          </Button>
          {passwordResponse && (
            <Typography
              color={passwordResponse.includes("Erro") ? "error" : "primary"}
              style={{ marginTop: "10px", textAlign: "center" }}
            >
              {passwordResponse}
            </Typography>
          )}
        </Box>
      )}
    </Container>
  );
};

export default App;
