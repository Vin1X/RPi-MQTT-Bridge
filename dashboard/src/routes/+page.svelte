<script>
	import { onMount } from 'svelte';
	import mqtt from 'mqtt';

	let status = $state('Verbinde...');
	let telemetry = $state({
		cpu: 0,
		ram: { available: 0 },
		wlan: { signal: 0 }
	});

	onMount(() => {
		const client = mqtt.connect('ws://192.168.178.144:9001/mqtt');

		client.on('connect', () => {
			status = 'Verbunden';
			client.subscribe('telemetry');
		});

		client.on('message', (topic, message) => {
			try {
				telemetry = JSON.parse(message.toString());
			} catch (e) {
				console.error('Payload Fehler', e);
			}
		});
		return () => client.end();
	});
</script>

<p>
	Status: <span style="color: {status === 'Verbunden' ? 'green' : 'red'}">{status}</span>
</p>

<div class="columns-3 rounded-2xl border-2 p-4">
	<div class="mx-2 rounded-2xl border-2 p-4">
		<h3>CPU</h3>
		<div></div>
	</div>
	<div class="mx-2 rounded-2xl border-2 p-4"><h3>RAM</h3></div>
	<div class="mx-2 rounded-2xl border-2 p-4"><h3>WLAN</h3></div>
</div>

<div>
	<p>CPU: {telemetry.cpu.temp} °C</p>
	<p>CPU: {telemetry.cpu.usage} %</p>
	<p>RAM Free: {telemetry.ram.available} KB</p>
	<p>Signal: {telemetry.wlan.signal} dBm</p>
</div>
