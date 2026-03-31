<script lang="ts">
	import '../app.css';
	import { DarkMode } from 'flowbite-svelte';
	import favicon from '$lib/assets/favicon.svg';

	let { children } = $props();
</script>

<svelte:head>
	<link rel="icon" href={favicon} />
</svelte:head>

<div class="flex min-h-screen flex-col">
	<header
		class="flex h-16 w-full items-center justify-start gap-8 border-b border-gray-700 bg-gray-800/80 p-4 text-3xl font-semibold text-white"
	>
		<div class="block">Raspberry Pi Dashboard</div>

		<DarkMode class="text-primary-500 dark:text-primary-600 border dark:border-gray-800" />
	</header>

	<main class="animate-fade-in grow bg-white p-4">
		{@render children()}
	</main>
</div>
