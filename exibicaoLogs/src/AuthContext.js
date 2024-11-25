import React, { useState } from "react";
import { Button, Container, Box, TextField, Typography } from "@mui/material";
import { useAuth, AuthProvider } from "./AuthContext";
import LogViewer from "./LogViewer";
import SensorControl from "./SensorControl";

const App = () => {
  const { authToken, setAuthToken } = useAuth(); // Acessa o contexto de autenticação
  const [selectedView, setSelectedView] = useState("logs");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [authError, setAuthError] = useState("");

  const handleAuthSubmit = async () => {
    setAuthError("");
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
        setAuthToken(data.token); // Armazena o token no contexto
        alert("Autenticação bem-sucedida!");
      } else {
        const errorData = await response.json();
        setAuthError(errorData.message || "Erro ao autenticar.");
      }
    } catch (error) {
      console.error("Erro na autenticação:", error);
      setAuthError("Erro na conexão com o servidor.");
    }
  };

  const handleButtonClick = (view) => {
    setSelectedView(view);
  };

  return (
    <Container maxWidth="lg" style={{ paddingTop: "20px" }}>
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
        >
          Sensores
        </Button>
        <Button
          variant={selectedView === "auth" ? "contained" : "outlined"}
          color="primary"
          onClick={() => handleButtonClick("auth")}
        >
          Autenticar
        </Button>
      </Box>

      {selectedView === "logs" && <LogViewer />}
      {selectedView === "sensores" && <SensorControl />}
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
    </Container>
  );
};

// Encapsula o App com o AuthProvider
const WrappedApp = () => (
  <AuthProvider>
    <App />
  </AuthProvider>
);

export default WrappedApp;
