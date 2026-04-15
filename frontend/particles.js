class JarvisParticles {
  constructor(canvas) {
    this.canvas = canvas;
    this.ctx = canvas.getContext('2d');
    this.state = 'idle';
    this.audioLevel = 0;
    this.t = 0;
    this.particles = Array.from({ length: 240 }, (_, i) => ({
      angle: (Math.PI * 2 * i) / 240,
      radius: 130 + Math.random() * 60,
      speed: 0.002 + Math.random() * 0.004,
      jitter: Math.random() * Math.PI * 2
    }));

    this.resize();
    window.addEventListener('resize', () => this.resize());
    requestAnimationFrame(() => this.frame());
  }

  setState(state) { this.state = state; }
  setAudioLevel(level) { this.audioLevel = Math.max(0, Math.min(1, level)); }

  resize() {
    const dpr = window.devicePixelRatio || 1;
    this.canvas.width = this.canvas.clientWidth * dpr;
    this.canvas.height = this.canvas.clientHeight * dpr;
    this.ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    this.cx = this.canvas.clientWidth / 2;
    this.cy = this.canvas.clientHeight / 2;
  }

  frame() {
    const ctx = this.ctx;
    const w = this.canvas.clientWidth;
    const h = this.canvas.clientHeight;
    this.t += 1;

    ctx.clearRect(0, 0, w, h);

    const rotation = this.state === 'thinking' ? this.t * 0.01 : this.t * 0.002;
    const pulse = this.state === 'speaking' ? 16 : this.state === 'listening' ? 8 + this.audioLevel * 22 : 4;

    ctx.save();
    ctx.translate(this.cx, this.cy);
    ctx.rotate(rotation);

    for (const p of this.particles) {
      const wave = Math.sin(this.t * p.speed + p.jitter) * (6 + pulse);
      const r = p.radius + wave;
      const x = Math.cos(p.angle) * r;
      const y = Math.sin(p.angle) * r;
      const alpha = 0.45 + Math.sin(this.t * 0.02 + p.jitter) * 0.25;
      ctx.fillStyle = `rgba(56, 189, 248, ${alpha})`;
      ctx.beginPath();
      ctx.arc(x, y, 1.8 + pulse * 0.03, 0, Math.PI * 2);
      ctx.fill();
    }

    ctx.strokeStyle = 'rgba(34,211,238,0.35)';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(0, 0, 112 + pulse, 0, Math.PI * 2);
    ctx.stroke();

    ctx.restore();

    requestAnimationFrame(() => this.frame());
  }
}
