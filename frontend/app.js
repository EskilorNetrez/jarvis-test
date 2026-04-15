const stateLabel = document.getElementById('stateLabel');
const transcript = document.getElementById('transcript');
const response = document.getElementById('response');
const confirmButton = document.getElementById('confirmButton');
const particles = new JarvisParticles(document.getElementById('particleCanvas'));

function updateState(state) {
  stateLabel.textContent = state[0].toUpperCase() + state.slice(1);
  stateLabel.className = `state ${state}`;
  particles.setState(state);
  confirmButton.classList.toggle('hidden', state !== 'confirming');
}

confirmButton.addEventListener('click', () => {
  if (window.chrome?.webview) {
    window.chrome.webview.postMessage('confirm_action');
  }
});

function handleEvent(payload) {
  if (payload.state) updateState(payload.state);
  if (payload.transcript) transcript.textContent = payload.transcript;
  if (payload.response) response.textContent = payload.response;
  if (typeof payload.audioLevel === 'number') particles.setAudioLevel(payload.audioLevel);
}

if (window.chrome?.webview) {
  window.chrome.webview.addEventListener('message', (event) => {
    try {
      handleEvent(JSON.parse(event.data));
    } catch {
      // Ignore malformed events.
    }
  });
}
