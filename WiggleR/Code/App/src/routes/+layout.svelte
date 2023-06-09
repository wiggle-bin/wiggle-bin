<script lang="ts">
	import { start, end } from '../store/store';
	import { goto } from '$app/navigation';
	import { onMount } from 'svelte';
	import ImageSelector from '../lib/ImageSelector.svelte';
	import logo from '$lib/assets/wiggleLogo.svg';

	let openImageSelector: Boolean | 'start' | 'end' = false;
	let startTime = '';
	let endTime = '';

	function close() {
		openImageSelector = false;
	}

	function scheduleStart() {
		openImageSelector = 'start';
	}
	function scheduleEnd() {
		openImageSelector = 'end';
	}

	function imageSelected(e: CustomEvent) {
		if (openImageSelector === 'start') {
			start.update((start) => (start = e.detail));
		}
		if (openImageSelector === 'end') {
			end.update((end) => (end = e.detail));
		}
		openImageSelector = false;
	}

	onMount(async () => {
		await import('@kor-ui/kor');
		await import('@kor-ui/kor/kor-styles.css');
	});

	start.subscribe((start) => (startTime = start));
	end.subscribe((end) => (endTime = end));
</script>

<kor-page padding="var(--spacing-s)">
	<kor-app-bar slot="top" logo={logo}>
		<kor-tabs>
			<kor-tab-item active on:click={() => goto('/')} label="Dashboard" />
			<kor-tab-item on:click={() => goto('/timelapse')} label="Timelapse" />
			<kor-tab-item on:click={() => goto('/timeline')} label="Timeline" />
			<kor-tab-item on:click={() => goto('/test')} label="Test" />
		</kor-tabs>

		<div class="actionsBar" slot="functions">
			<kor-button class="double-button left" icon="schedule" label={startTime || 'Start'} on:click={scheduleStart} />
			<kor-button class="double-button right" icon="schedule" label={endTime || 'End'} on:click={scheduleEnd} />
		</div>
		{#if openImageSelector}
			<ImageSelector on:select={imageSelected} on:close={close} />
		{/if}
	</kor-app-bar>

	<slot />
</kor-page>

<style>
	.actionsBar {
		display: flex;
	}
	.double-button {
		color: white;
		border-bottom: black solid 1px;
	}
	.double-button.left {
		border-right: 1px solid white;
		border-radius: 7px 0 0 7px;
	}
	.double-button.right {
		border-radius: 0  7px 7px 0;
	}
	.double-button:hover {
		border-bottom: white solid 1px;
	}
</style>
